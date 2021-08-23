import { ESPLoader } from "espc3-web-flasher";
export declare const getChipFamilyName: (esploader: ESPLoader) => "ESP32" | "ESP8266" | "ESP32-S2" | "ESP32-C3" | "Unknown Chip";
export declare const sleep: (time: number) => Promise<unknown>;
export declare const fireEvent: <Event_1 extends keyof HTMLElementEventMap>(eventTarget: EventTarget, type: Event_1, detail?: HTMLElementEventMap[Event_1]["detail"] | undefined, options?: {
    bubbles?: boolean | undefined;
    cancelable?: boolean | undefined;
    composed?: boolean | undefined;
} | undefined) => void;
