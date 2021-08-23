import { flash } from "./flash";
import "./flash-log";
import "./flash-progress";
let stateListenerAdded = false;
let logEl;
let progressEl;
let improvEl;
const addElement = (button, element) => {
    button.renderRoot.append(element);
    return element;
};
export const startFlash = async (button) => {
    if (button.hasAttribute("active")) {
        return;
    }
    const manifest = button.manifest || button.getAttribute("manifest");
    if (!manifest) {
        alert("No manifest defined!");
        return;
    }
    let hasImprov = false;
    if (!stateListenerAdded) {
        stateListenerAdded = true;
        button.addEventListener("state-changed", (ev) => {
            var _a;
            const state = (button.state = ev.detail);
            if (state.state === "initializing" /* INITIALIZING */) {
                button.toggleAttribute("active", true);
            }
            else if (state.state === "manifest" /* MANIFEST */ && ((_a = state.build) === null || _a === void 0 ? void 0 : _a.improv)) {
                hasImprov = true;
                // @ts-ignore
                // preload improv button
                import("https://www.improv-wifi.com/sdk-js/launch-button.js");
            }
            else if (state.state === "finished" /* FINISHED */) {
                button.toggleAttribute("active", false);
                if (hasImprov) {
                    startImprov(button);
                }
            }
            else if (state.state === "error" /* ERROR */) {
                button.toggleAttribute("active", false);
            }
            progressEl === null || progressEl === void 0 ? void 0 : progressEl.processState(ev.detail);
            logEl === null || logEl === void 0 ? void 0 : logEl.processState(ev.detail);
        });
    }
    const logConsole = button.logConsole || button.hasAttribute("log-console");
    const showLog = button.showLog || button.hasAttribute("show-log");
    const showProgress = !showLog &&
        button.hideProgress !== true &&
        !button.hasAttribute("hide-progress");
    if (showLog && !logEl) {
        logEl = addElement(button, document.createElement("esp-web-flash-log"));
    }
    else if (!showLog && logEl) {
        logEl.remove();
        logEl = undefined;
    }
    if (showProgress && !progressEl) {
        progressEl = addElement(button, document.createElement("esp-web-flash-progress"));
    }
    else if (!showProgress && progressEl) {
        progressEl.remove();
        progressEl = undefined;
    }
    logEl === null || logEl === void 0 ? void 0 : logEl.clear();
    progressEl === null || progressEl === void 0 ? void 0 : progressEl.clear();
    improvEl === null || improvEl === void 0 ? void 0 : improvEl.classList.toggle("hidden", true);
    flash(button, logConsole
        ? console
        : {
            log: () => { },
            error: () => { },
            debug: () => { },
        }, manifest, button.eraseFirst !== undefined
        ? button.eraseFirst
        : button.hasAttribute("erase-first"));
};
const startImprov = async (button) => {
    // @ts-ignore
    await import("https://www.improv-wifi.com/sdk-js/launch-button.js");
    const improvButtonConstructor = customElements.get("improv-wifi-launch-button");
    if (!improvButtonConstructor.isSupported ||
        !improvButtonConstructor.isAllowed) {
        return;
    }
    if (!improvEl) {
        improvEl = document.createElement("improv-wifi-launch-button");
        improvEl.addEventListener("state-changed", (ev) => {
            if (ev.detail.state === "PROVISIONED") {
                improvEl.classList.toggle("hidden", true);
            }
        });
        const improvButton = document.createElement("button");
        improvButton.slot = "activate";
        improvButton.textContent = "CLICK HERE TO FINISH SETTING UP YOUR DEVICE";
        improvEl.appendChild(improvButton);
        addElement(button, improvEl);
    }
    improvEl.classList.toggle("hidden", false);
};
