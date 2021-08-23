import { LitElement } from "lit";
import { FlashState } from "./const";
import "@material/mwc-linear-progress";
export declare class FlashProgress extends LitElement {
    private _state?;
    private _indeterminate;
    private _progress;
    processState(state: FlashState): void;
    clear(): void;
    protected render(): import("lit").TemplateResult<1> | undefined;
    static styles: import("lit").CSSResultGroup;
}
declare global {
    interface HTMLElementTagNameMap {
        "esp-web-flash-progress": FlashProgress;
    }
}
