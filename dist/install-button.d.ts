import { FlashState } from "./const";
export declare class InstallButton extends HTMLElement {
    static isSupported: boolean;
    static isAllowed: boolean;
    private static style;
    manifest?: string;
    eraseFirst?: boolean;
    hideProgress?: boolean;
    showLog?: boolean;
    logConsole?: boolean;
    state?: FlashState;
    renderRoot?: ShadowRoot;
    static preload(): void;
    connectedCallback(): void;
}
