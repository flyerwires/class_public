/*
 * Urformel perturbation stubs for CLASS.
 *
 * This file provides guidance for adding the Urformel scalar field and its
 * coupling to the dark matter component into the linear perturbations
 * module of CLASS.  The actual implementation in `perturbations.c` is
 * considerably more involved; here we outline the key modifications.
 *
 * In coupled quintessence models, the dark matter energy–momentum is
 * exchanged with the scalar field.  At the background level this appears
 * through the term −3 c ρ_m u in the continuity equation for ρ_m and the
 * source term +3 c ρ_m in the Klein–Gordon equation.  On the level of
 * first‑order perturbations, similar terms couple δρ_c and δΦ.
 */

#include "background_urformel.h"

/*
 * In the synchronous gauge used in CLASS, the perturbation equations for
 * dark matter and the scalar field should be modified as follows:
 *
 *  1. Dark matter continuity equation:
 *       δρ_c' + 3 H δρ_c + (ρ_c + p_c) (θ_c + ½ h')
 *       = - 3 c ρ_c δu
 *
 *     Here, δu is the perturbation of the dimensionless field derivative u.
 *
 *  2. Dark matter Euler equation:
 *       θ_c' + H θ_c = + c k^2 δΦ / a^2
 *
 *     where k is the wavenumber and δΦ is the scalar field perturbation.
 *
 *  3. Scalar field perturbation equation:
 *       δΦ'' + 3 H δΦ' + (k^2 / a^2 + d^2V/dΦ^2) δΦ
 *       = 4 Φ' ψ' - α (δρ_c)
 *
 *     In synchronous gauge, ψ is the metric potential (one of the two
 *     Einstein potentials).  In CLASS conventions this equation must be
 *     expressed in terms of the synchronous variables (h, η).
 *
 * In practice, adding these equations requires:
 *   • Introducing new perturbation variables for δΦ and δu.
 *   • Adding source terms proportional to the coupling c to the existing
 *     dark matter equations.
 *   • Computing the effective mass squared d^2V/dΦ^2 = µ² for the field.
 *   • Providing appropriate initial conditions (usually adiabatic).
 *
 * Because these modifications are lengthy and depend on the internal
 * structure of CLASS, this file does not implement them directly.  Instead,
 * it gives a starting point for where to insert the changes.  Consult the
 * CLASS perturbation module documentation and the coupled quintessence
 * literature for the full set of equations.
 */

/* Placeholder function demonstrating where modifications might be placed. */
void
urformel_modify_perturbations(void)
{
  /*
   * In your fork of CLASS, locate the section of `perturbations.c` that
   * initializes and evolves the perturbations.  Define new indices for
   * δΦ and δu in the perturbation vector `y`.  Then augment the
   * differential equations with the coupling terms shown above.
   */
}
