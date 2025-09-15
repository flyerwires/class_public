# Anleitung zur Einbindung von CLASS/CAMB für die Urformel

Dieses Dokument skizziert einen möglichen Weg, wie die Urformel‐Gleichungen in die
gängigen Boltzmann‐Codes **CLASS** bzw. **CAMB** integriert werden können, um
kosmologische Präzisionsdaten (z.\u202fB. Planck, DESI, Pantheon+) zu analysieren.

Die Schritte sollten als Leitfaden dienen; die tatsächliche Implementierung erfordert
Erfahrung mit C/C++ und Fortran sowie ein Verständnis der CLASS/CAMB‐Architektur.

## 1. Vorbereitung

1. **Fork des CLASS‑Repositories**: Klone das offizielle `class_public` Repository in
   deinen Account bzw. als Submodul in dieses Projekt.  Achte darauf, die Lizenz
   (GPL) zu respektieren.

2. **Abhängigkeiten installieren**: CLASS benötigt einen C– und Fortran‑Compiler
   (gcc/gfortran), FFTW und GSL.  Siehe offizielle CLASS‑Dokumentation.

3. **Branch anlegen**: Erstelle einen neuen Branch (z.\u202fB. `urformel-class`), in dem
   du die folgenden Änderungen vornimmst.

## 2. Hintergrundgleichungen (background.c/h)

Unsere Urformel‑Kosmologie erweitert CLASS um eine gekoppeltes Skalarfeld mit
Potenzial und Kopplung:

\[
\mathcal{L}_U = \frac{M^2}{2}R - \frac{1}{2}\partial_\mu \Phi \partial^\mu \Phi
                - V(\Phi) + \alpha\,\rho_m\,\Phi.
\]

Zur Implementierung empfiehlt sich folgendes Vorgehen:

1. **Neue Felder definieren**: Füge in `background.h` zusätzliche Variablen für
   `x = \Phi/M`, `u = dx/dN` und deren Anfangswerte hinzu.  Ändere die Struktur
   `background` entsprechend.

2. **Gleichungen implementieren**: In `background.c` muss die Funktion
   `background_derivs()` um die Gleichungen für `x`, `u` und den normierten Hubble‑Faktor
   `E` erweitert werden.  Die Ableitungen lauten (siehe vorherige Diskussion):

   ```c
   dx_dN = u;
   dE_dN = -0.5 * E * (4*Omega_r + 3*Omega_m + u*u - 3*c*x*Omega_m);
   du_dN = -(3 + dE_dN/E)*u - W_x / (E*E)
           + 3*c * Omega_m0 * pow(a,-3) / (E*E);
   ```

   wobei `W_x = dV/dx` und `c = α M` die Kopplungsstärke ist.

3. **Friedmann‑Constraint anpassen**: Passe die Berechnung von `H` so an,
   dass die kinetische Energie `u` und der Kopplungsterm korrekt in
   `rho_phi` eingehen (siehe Gleichungen im Chat).

4. **Parameter einlesen**: Erweitere `input.c`, sodass neue Parameter `mu` (m/H0),
   `c` und optionale Startwerte `x_ini`, `u_ini` eingelesen werden können.

## 3. Störungsgleichungen (perturbations.c/h)

Für die Analyse der CMB‑Anisotropien müssen auch die Störungsgleichungen
angepasst werden:

1. **Skalarfeld‑Perturbationen**: Füge die Störung `\u03b4\u03a6` ein und berechne
   deren Entwicklungsgleichung.  Die Kopplung führt zu einem Quellterm
   proportional zu `α \u03b4\u03c1_m`.

2. **Gekoppelte Materie**: Modifiziere die Kontinuitätsgleichung der dunklen
   Materie um den Term `-3c \u03c1_m u` (Hintergrund) bzw. für die Störungen
   `-3c \u03c1_m \u03b4u`.

3. **Initialbedingungen**: Achte darauf, konsistente adiabatische
   Anfangsbedingungen zu setzen.  In vielen Fällen können Standardbedingungen
   übernommen werden, wenn das Feld am Anfang potenzialdominiert ist.

## 4. Build‑System

Der bestehende GitHub‑Actions‑Workflow `class-ci.yml` baut bereits das
offizielle CLASS.  Sobald du deine Anpassungen vorgenommen hast,
aktualisiere die `ci/smoke_run.py` oder schreibe eigene Tests, die den
angepassten CLASS‑Code als Bibliothek verwenden.

## 5. Schnittstelle zu MontePython oder Cobaya

1. **Konfigurationsdatei erstellen**: Für einen MCMC‑Fit benötigst du eine
   `params/urformel.yaml` (Cobaya) oder `.ini`‑Datei (MontePython) mit den
   neuen Parametern `mu`, `c`, `x_ini` usw.  Setze sinnvolle Prioren.

2. **Likelihoods binden**: Füge Planck‑ und DESI‑Likelihoods hinzu.  Beide
   Frameworks bieten offizielle Packages (`plc_3.0` etc.), die du in den
   Docker/CI‑Workflow laden musst.

3. **MCMC starten**: Richte einen separaten GitHub‑Action‑Job ein (z.\u202fB.
   `class-mcmc.yml`), der Cobaya oder MontePython ausführt und die Posterioren
   speichert.  Für schnelle Tests genügt ein kurzer Lauf mit wenigen
   Samples; für wissenschaftliche Auswertungen sind längere Ketten
   erforderlich.

## 6. Weiteres Vorgehen

Die hier beschriebenen Schritte bilden einen möglichen Einstieg.  Für eine
vollständige Implementation empfiehlt sich, zunächst die Hintergrundgleichungen
zu implementieren und in `ci/smoke_run.py` gegen bekannte Lösungen zu
testen.  Anschließend können die Störungsgleichungen ergänzt und mit Hilfe
von Cobaya/MontePython Fit‑Runs gegen realistische Daten durchgeführt
werden.

Falls du Fragen hast oder auf Probleme stößt, kann ich anhand konkreter
Dateiausschnitte oder Fehlermeldungen weiterhelfen.
