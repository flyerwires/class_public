/*
 * Urformel additions to the CLASS background module.
 *
 * This header declares the additional variables and parameters required
 * for the Urformel scalar field cosmology.  They are intended to be
 * integrated into the existing CLASS `background` structure and related
 * function prototypes.  Use this file as a guideline when editing
 * `background.h` in your fork of CLASS.  It does not compile on its
 * own but shows which fields and parameters should be added.
 *
 * Variables:
 *   x    – dimensionless scalar field variable, defined as Φ/M where M is
 *          the reduced Planck mass.  It evolves according to dx/dN = u.
 *   u    – derivative of x with respect to the number of e‑folds N=ln(a).
 *         In other words, u = dx/dN.
 *
 * Parameters:
 *   mu   – dimensionless mass parameter of the field, μ = m/H0 where m is
 *          the scalar field mass and H0 is the Hubble constant today.
 *   c    – dimensionless coupling constant, c = α·M.  A positive value
 *          slightly increases the effective mass of the field and slows
 *          the expansion; a negative value can lead to instability.
 *   x_ini – initial value of x at the starting scale factor a_ini.
 *
 * To integrate these into CLASS, locate the `background` struct in
 * `background.h` and add the new members below.  Likewise, extend the
 * parameter reading routines in `input.c` to accept `mu`, `c` and
 * `x_ini` as new input parameters.
 */

#ifndef URFORMEL_BACKGROUND_H
#define URFORMEL_BACKGROUND_H

/* Additional fields for the background structure */
double urformel_x;      /* scalar field x = Φ/M */
double urformel_u;      /* derivative dx/dN */
double urformel_mu;     /* mass parameter μ = m/H0 */
double urformel_c;      /* coupling constant c = α M */
double urformel_x_ini;  /* initial value of x at a_ini */

#endif /* URFORMEL_BACKGROUND_H */
