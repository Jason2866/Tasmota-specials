var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
import { css, html, LitElement } from "lit";
import { customElement, state } from "lit/decorators.js";
import "@material/mwc-linear-progress";
import { classMap } from "lit/directives/class-map.js";
let FlashProgress = class FlashProgress extends LitElement {
    constructor() {
        super(...arguments);
        this._indeterminate = true;
        this._progress = 0;
    }
    processState(state) {
        this._state = state;
        if (this._state.state === "writing" /* WRITING */) {
            this._indeterminate = false;
            this._progress = this._state.details.percentage / 100;
        }
        if (this._state.state === "error" /* ERROR */) {
            this._indeterminate = false;
        }
    }
    clear() {
        this._state = undefined;
        this._progress = 0;
        this._indeterminate = true;
    }
    render() {
        if (!this._state) {
            return;
        }
        return html `<h2
        class=${classMap({
            error: this._state.state === "error" /* ERROR */,
            done: this._state.state === "finished" /* FINISHED */,
        })}
      >
        ${this._state.message}
      </h2>
      <p>
        ${this._state.manifest
            ? html `${this._state.manifest.name}: ${this._state.chipFamily}`
            : html `&nbsp;`}
      </p>
      <mwc-linear-progress
        class=${classMap({
            error: this._state.state === "error" /* ERROR */,
            done: this._state.state === "finished" /* FINISHED */,
        })}
        .indeterminate=${this._indeterminate}
        .progress=${this._progress}
      ></mwc-linear-progress>`;
    }
};
FlashProgress.styles = css `
    :host {
      display: block;
      --mdc-theme-primary: var(--esp-tools-progress-color, #03a9f4);
    }
    .error {
      color: var(--esp-tools-error-color, #dc3545);
      --mdc-theme-primary: var(--esp-tools-error-color, #dc3545);
    }
    .done {
      color: var(--esp-tools-success-color, #28a745);
      --mdc-theme-primary: var(--esp-tools-success-color, #28a745);
    }
    mwc-linear-progress {
      text-align: left;
    }
    h2 {
      margin: 16px 0 0;
    }
    p {
      margin: 4px 0;
    }
  `;
__decorate([
    state()
], FlashProgress.prototype, "_state", void 0);
__decorate([
    state()
], FlashProgress.prototype, "_indeterminate", void 0);
__decorate([
    state()
], FlashProgress.prototype, "_progress", void 0);
FlashProgress = __decorate([
    customElement("esp-web-flash-progress")
], FlashProgress);
export { FlashProgress };
