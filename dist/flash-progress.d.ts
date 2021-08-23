import { LitElement } from "lit";
import { FlashState } from "./const";
import "@material/mwc-linear-progress";
export declare class FlashProgress extends LitElement {
    private _state?;
    private _indeterminate;
    private _progress;
    processState(state: FlashState): void;
    clear(): void;
    protected render(): any;
    static styles: any;
}
declare global {
    interface HTMLElementTagNameMap {
        "esp-web-flash-progress": FlashProgress;
    }
}
