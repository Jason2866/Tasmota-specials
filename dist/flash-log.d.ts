import { HTMLTemplateResult, LitElement } from "lit";
import { FlashState, State } from "./const";
interface Row {
    state?: State;
    message: HTMLTemplateResult | string;
    error?: boolean;
    action?: boolean;
}
export declare class FlashLog extends LitElement {
    private _rows;
    protected render(): import("lit").TemplateResult<1>;
    willUpdate(): void;
    clear(): void;
    processState(state: FlashState): void;
    /**
     * Add or replace a row.
     */
    addRow(row: Row): void;
    /**
     * Add an error row
     */
    addError(message: Row["message"]): void;
    /**
     * Add an action row
     */
    addAction(message: Row["message"]): void;
    /**
     * Remove last row if state matches
     */
    removeRow(state: string): void;
    static styles: import("lit").CSSResultGroup;
}
declare global {
    interface HTMLElementTagNameMap {
        "esp-web-flash-log": FlashLog;
    }
}
export {};
