/*
 * Urformel background derivative functions for CLASS.
 *
 * This file illustrates how to incorporate the Urformel scalar field
 * dynamics into the CLASS background module.  It is not a drop‑in
 * replacement for any existing file but rather serves as a template for
 * the modifications that should be made to `background.c`.  See the
 * accompanying README_CLASS_URFORMEL.md for a detailed description of
 * the variables and equations.
 *
 * The key idea is to promote the scalar field variable x = Φ/M and its
 * derivative u = dx/dN to dynamical variables tracked alongside the
 * existing cosmological variables.  The derivatives are then computed
 * according to the Urformel field equations in e‑fold time N = ln(a).
 */

#include <math.h>
#include "background_urformel.h"

/*
 * Compute the derivative of the Urformel variables with respect to N.
 *
 * Arguments:
 *   a         – current scale factor (exp(N))
 *   Omega_r   – fraction of radiation at the current epoch
 *   Omega_m   – fraction of total matter (baryons + DM) at the current epoch
 *   x         – current value of the scalar field (dimensionless)
 *   u         – current derivative of the field with respect to N
 *   mu        – mass parameter μ = m/H0
 *   c         – coupling constant c = α·M
 *
 * Returns:
 *   dx_dN, du_dN and dE_dN (derivative of the dimensionless Hubble rate E)
 *   through output pointers.  The calling routine is responsible for
 *   computing E^2 via the modified Friedmann constraint:
 *
 *     E^2 (1 - u^2/6) = Ω_r a^{-4} + Ω_m a^{-3} (1 - c x) + W/3
 *
 *   where W = ½ μ² x² is the dimensionless potential.
 */
static void
urformel_compute_derivs(double a,
                        double Omega_r,
                        double Omega_m,
                        double x,
                        double u,
                        double mu,
                        double c,
                        double E,
                        double *dx_dN,
                        double *du_dN,
                        double *dE_dN)
{
  /* Compute derivative of the potential W(x) with respect to x */
  double W_x = mu * mu * x;         /* for a quadratic potential W=½ μ² x² */
  double Omega_m0_a3 = Omega_m * pow(a, -3.0);

  /* Compute dE/E first (Raychaudhuri equation in e‑folds) */
  *dE_dN = -0.5 * E * (4.0 * Omega_r + 3.0 * Omega_m + u * u - 3.0 * c * x * Omega_m);

  /* Derivatives of x and u */
  *dx_dN = u;
  *du_dN = -(3.0 + *dE_dN / E) * u - W_x / (E * E)
           + 3.0 * c * Omega_m0_a3 / (E * E);
}

/*
 * Example wrapper to update the Urformel field within the CLASS background
 * derivative routine.  In your actual CLASS fork, incorporate these
 * computations directly into the `background_derivs()` function, ensuring
 * that y[] and dy[] are extended to include x and u.
 */
void
urformel_update(double a,
                double Omega_r,
                double Omega_m,
                struct background *pba,
                double *y,
                double *dy)
{
  /* Indices of x and u in y[] */
  int index_x = pba->bg_index_urformel_x;
  int index_u = pba->bg_index_urformel_u;
  int index_E = pba->bg_index_H; /* existing index of E=H/H0 in CLASS */

  double x = y[index_x];
  double u = y[index_u];
  double E = y[index_E];

  double dx_dN, du_dN, dE_dN;
  urformel_compute_derivs(a, Omega_r, Omega_m, x, u, pba->urformel_mu,
                          pba->urformel_c, E, &dx_dN, &du_dN, &dE_dN);

  /* Store derivatives back into dy array */
  dy[index_x] = dx_dN;
  dy[index_u] = du_dN;
  dy[index_E] = dE_dN;
}
