import { connect } from "espc3-web-flasher";
import { fireEvent, getChipFamilyName, sleep } from "./util";
export const flash = async (eventTarget, logger, manifestPath, eraseFirst) => {
    let manifest;
    let build;
    let chipFamily;
    const fireStateEvent = (stateUpdate) => {
        fireEvent(eventTarget, "state-changed", {
            ...stateUpdate,
            manifest,
            build,
            chipFamily,
        });
    };
    const manifestURL = new URL(manifestPath, location.toString()).toString();
    const manifestProm = fetch(manifestURL).then((resp) => resp.json());
    let esploader;
    try {
        esploader = await connect(logger);
    }
    catch (err) {
        // User pressed cancel on web serial
        return;
    }
    // For debugging
    window.esploader = esploader;
    fireStateEvent({
        state: "initializing" /* INITIALIZING */,
        message: "Initializing...",
        details: { done: false },
    });
    try {
        await esploader.initialize();
    }
    catch (err) {
        logger.error(err);
        if (esploader.connected) {
            fireStateEvent({
                state: "error" /* ERROR */,
                message: "Failed to initialize. Try resetting your device or holding the BOOT button while selecting your serial port.",
                details: { error: "failed_initialize" /* FAILED_INITIALIZING */, details: err },
            });
            await esploader.disconnect();
        }
        return;
    }
    chipFamily = getChipFamilyName(esploader);
    fireStateEvent({
        state: "initializing" /* INITIALIZING */,
        message: `Initialized. Found ${chipFamily}`,
        details: { done: true },
    });
    fireStateEvent({
        state: "manifest" /* MANIFEST */,
        message: "Fetching manifest...",
        details: { done: false },
    });
    try {
        manifest = await manifestProm;
    }
    catch (err) {
        fireStateEvent({
            state: "error" /* ERROR */,
            message: `Unable to fetch manifest: ${err.message}`,
            details: { error: "fetch_manifest_failed" /* FAILED_MANIFEST_FETCH */, details: err },
        });
        await esploader.disconnect();
        return;
    }
    build = manifest.builds.find((b) => b.chipFamily === chipFamily);
    fireStateEvent({
        state: "manifest" /* MANIFEST */,
        message: `Found manifest for ${manifest.name}`,
        details: { done: true },
    });
    if (!build) {
        fireStateEvent({
            state: "error" /* ERROR */,
            message: `Your ${chipFamily} board is not supported.`,
            details: { error: "not_supported" /* NOT_SUPPORTED */, details: chipFamily },
        });
        await esploader.disconnect();
        return;
    }
    fireStateEvent({
        state: "preparing" /* PREPARING */,
        message: "Preparing installation...",
        details: { done: false },
    });
    const filePromises = build.parts.map(async (part) => {
        const url = new URL(part.path, manifestURL).toString();
        const resp = await fetch(url);
        if (!resp.ok) {
            throw new Error(`Downlading firmware ${part.path} failed: ${resp.status}`);
        }
        return resp.arrayBuffer();
    });
    // Run the stub while we wait for files to download
    const espStub = await esploader.runStub();
    const files = [];
    let totalSize = 0;
    for (const prom of filePromises) {
        try {
            const data = await prom;
            files.push(data);
            totalSize += data.byteLength;
        }
        catch (err) {
            fireStateEvent({
                state: "error" /* ERROR */,
                message: err,
                details: { error: "failed_firmware_download" /* FAILED_FIRMWARE_DOWNLOAD */, details: err },
            });
            await esploader.disconnect();
            return;
        }
    }
    fireStateEvent({
        state: "preparing" /* PREPARING */,
        message: "Installation prepared",
        details: { done: true },
    });
    if (eraseFirst) {
        fireStateEvent({
            state: "erasing" /* ERASING */,
            message: "Erasing device...",
            details: { done: false },
        });
        await espStub.eraseFlash();
        fireStateEvent({
            state: "erasing" /* ERASING */,
            message: "Device erased",
            details: { done: true },
        });
    }
    let lastPct = 0;
    fireStateEvent({
        state: "writing" /* WRITING */,
        message: `Writing progress: ${lastPct}%`,
        details: {
            bytesTotal: totalSize,
            bytesWritten: 0,
            percentage: lastPct,
        },
    });
    let totalWritten = 0;
    for (const part of build.parts) {
        const file = files.shift();
        try {
            await espStub.flashData(file, (bytesWritten) => {
                const newPct = Math.floor(((totalWritten + bytesWritten) / totalSize) * 100);
                if (newPct === lastPct) {
                    return;
                }
                lastPct = newPct;
                fireStateEvent({
                    state: "writing" /* WRITING */,
                    message: `Writing progress: ${newPct}%`,
                    details: {
                        bytesTotal: totalSize,
                        bytesWritten: totalWritten + bytesWritten,
                        percentage: newPct,
                    },
                });
            }, part.offset, true);
        }
        catch (err) {
            fireStateEvent({
                state: "error" /* ERROR */,
                message: err,
                details: { error: "write_failed" /* WRITE_FAILED */, details: err },
            });
            await esploader.disconnect();
            return;
        }
        totalWritten += file.byteLength;
    }
    fireStateEvent({
        state: "writing" /* WRITING */,
        message: "Writing complete",
        details: {
            bytesTotal: totalSize,
            bytesWritten: totalWritten,
            percentage: 100,
        },
    });
    await sleep(100);
    await esploader.hardReset();
    await esploader.disconnect();
    fireStateEvent({
        state: "finished" /* FINISHED */,
        message: "All done!",
    });
};
