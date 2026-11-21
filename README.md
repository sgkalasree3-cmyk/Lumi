# LUMI – AI Wellness Companion
*Healthy Today. Brighter Tomorrow.*

A self-contained, installable web app (PWA) built with plain HTML/CSS/JS. No build step, no dependencies, no server code.

## Files
| File | Purpose |
|---|---|
| `index.html` | The whole app (UI, logic, styles) |
| `manifest.webmanifest`, `icon.svg` | PWA install info and icon |
| `sw.js` | Service worker (offline app shell) |
| `esp32_lumi_sensor.ino` | Example ESP32 REST endpoint (stub sensors) |

## Run it on an Android phone (no Mac needed)

### Option A – Fastest: host it over HTTPS (demo mode works fully)
1. Put all files in one folder and upload it to a static host, e.g. **Netlify Drop** (drop the folder at app.netlify.com/drop) or **GitHub Pages**.
2. Open the URL in **Chrome on Android**.
3. Menu (⋮) → **Add to Home screen / Install app**.

> Limit: a page served over HTTPS cannot call an `http://` ESP32 (browser "mixed content" block). Use Option B for live ESP32 testing.

### Option B – Local HTTP (needed for live ESP32 data)
Serve the folder over plain HTTP on your own network:
- **On a PC (any OS):** `cd` into the folder and run `python3 -m http.server 8080`, then open `http://<pc-ip>:8080` on the phone (same Wi-Fi).
- **On the phone only:** install **Termux**, then `pkg install python`, `cd` to the folder, `python -m http.server 8080`, and open `http://localhost:8080` in Chrome.

Service workers/install only work on HTTPS or `localhost`, so with `http://<pc-ip>` the app still runs but may not be installable.

### Option C – Open `index.html` directly
May work in a phone file manager/browser; camera upload and storage can be restricted on `file://`. Not recommended.

## Using the app
- **Demo mode (default):** simulated values, labelled **DEMO DATA** on the banner and every card. They are never real measurements.
- **Live mode:** Device tab → enter ESP32 address → **Save & test**. If nothing valid arrives, LUMI shows "—"/"No data" (it never falls back to demo numbers).
- **Skin:** Take/upload a photo. Simple on-device image statistics are shown as *experimental visible observations*. No diagnosis, no upload.
- **Assistant:** local rules-based helper (no cloud AI). Explains available readings, gives general wellness tips, refuses diagnoses, and points to emergency services for urgent symptoms.
- **Wellness Assessment (Home → Wellness Assessment card):** 20 multiple-choice questions (one at a time, with "Question N of 20", Previous/Next, auto-saved progress you can resume). Afterwards it builds a summary from your actual answers: habit score, per-area bars, strengths, focus areas and general non-medical suggestions. Results are stored on the phone, can be reopened later, and exported with **Save as PDF / Print**. It is self-reported habits only: no diagnosis, and it does not use sensor readings.
- **Reports:** save snapshots (stored in the browser's localStorage), view/delete them. 3 clearly labelled **DEMO SAMPLE** reports are pre-loaded.
  - **PDF:** uses the browser print dialog → "Save as PDF".
  - **Email (requires email configuration):** opens your phone's email app via `mailto:` with the text pre-filled. No attachment; LUMI cannot send email itself.

## ESP32 REST API
LUMI polls `GET http://<esp32-ip>/api/sensors`:
```json
{
  "device": "ESP32-LUMI",
  "heart_rate": 72,
  "spo2": 98,
  "temperature": 36.5,
  "humidity": 48,
  "gas_status": "good",
  "gas_value": 120
}
```
Fields may be `null` or omitted. Out-of-range values are ignored. The ESP32 must send `Access-Control-Allow-Origin: *`. See `esp32_lumi_sensor.ino` (sensor functions are stubs that return nothing until you add real sensor code).

## Honest status (what has and hasn't been verified)
- Written and syntax-checked only; **not yet run on a real Android phone or browser by the author.** Please test each screen.
- **ESP32 connectivity: implemented in code but NOT tested** with real hardware.
- **Skin analysis: implemented** (brightness/texture/glare statistics) but **not validated**; it is not a clinical tool.
- **Email: depends on a configured email app;** delivery is not verified.
- **PDF: depends on the browser's print-to-PDF;** not tested across devices.
- SpO₂ and heart rate accuracy depend entirely on your sensors; LUMI only displays what it receives.
- General wellness information only. Not a medical device. Does not diagnose, treat or prevent disease.
