var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
import { css, html, LitElement } from "lit";
import { customElement, state } from "lit/decorators.js";
import { classMap } from "lit/directives/class-map.js";
let FlashLog = class FlashLog extends LitElement {
    constructor() {
        super(...arguments);
        this._rows = [];
    }
    render() {
        return html `${this._rows.map((row) => html `<div
          class=${classMap({
            error: row.error === true,
            action: row.action === true,
        })}
        >
          ${row.message}
        </div>`)}`;
    }
    willUpdate() {
        this.toggleAttribute("hidden", !this._rows.length);
    }
    clear() {
        this._rows = [];
    }
    processState(state) {
        if (state.state === "error" /* ERROR */) {
            this.addError(state.message);
            return;
        }
        this.addRow(state);
        if (state.state === "finished" /* FINISHED */) {
            this.addAction(html `<button @click=${this.clear}>Close this log</button>`);
        }
    }
    /**
     * Add or replace a row.
     */
    addRow(row) {
        // If last entry has same ID, replace it.
        if (row.state &&
            this._rows.length > 0 &&
            this._rows[this._rows.length - 1].state === row.state) {
            const newRows = this._rows.slice(0, -1);
            newRows.push(row);
            this._rows = newRows;
        }
        else {
            this._rows = [...this._rows, row];
        }
    }
    /**
     * Add an error row
     */
    addError(message) {
        this.addRow({ message, error: true });
    }
    /**
     * Add an action row
     */
    addAction(message) {
        this.addRow({ message, action: true });
    }
    /**
     * Remove last row if state matches
     */
    removeRow(state) {
        if (this._rows.length > 0 &&
            this._rows[this._rows.length - 1].state === state) {
            this._rows = this._rows.slice(0, -1);
        }
    }
};
FlashLog.styles = css `
    :host {
      display: block;
      margin-top: 16px;
      padding: 12px 16px;
      font-family: monospace;
      background: var(--esp-tools-log-background, black);
      color: var(--esp-tools-log-text-color, greenyellow);
      font-size: 14px;
      line-height: 19px;
    }

    :host([hidden]) {
      display: none;
    }

    button {
      background: none;
      color: inherit;
      border: none;
      padding: 0;
      font: inherit;
      text-align: left;
      text-decoration: underline;
      cursor: pointer;
    }

    .error {
      color: var(--esp-tools-error-color, #dc3545);
    }

    .error,
    .action {
      margin-top: 1em;
    }
  `;
__decorate([
    state()
], FlashLog.prototype, "_rows", void 0);
FlashLog = __decorate([
    customElement("esp-web-flash-log")
], FlashLog);
export { FlashLog };
