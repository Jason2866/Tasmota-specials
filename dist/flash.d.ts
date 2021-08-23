import { Logger } from "espc3-web-flasher";
export declare const flash: (eventTarget: EventTarget, logger: Logger, manifestPath: string, eraseFirst: boolean) => Promise<void>;
