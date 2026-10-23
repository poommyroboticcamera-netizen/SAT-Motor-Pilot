# Third-party notices and asset review

This is an engineering inventory of the retained source, not a completed redistribution clearance.

## Base application

MotorLab is a modified build of VESC Tool 7.00 by Benjamin Vedder and contributors. Pinned upstream commit: `01d5f10901116c311e3fb84d5a1541f663d3ce20`. Upstream: https://github.com/vedderb/vesc_tool . Source copyright headers specify GPL version 3 or later. The full license is retained at `source/LICENSE`. This software is provided WITHOUT ANY WARRANTY. Preserve upstream notices and deliver complete corresponding source, modifications, and build instructions when distributing binaries under the applicable GPL terms. VESC is an upstream mark; MotorLab does not claim affiliation or endorsement.

## Bundled libraries and fonts

Retain the full license texts in their original paths:

| Component | Evidence in retained source |
|---|---|
| QCodeEditor | `source/QCodeEditor/LICENSE.MIT`, MIT, Megaxela |
| qmarkdowntextedit | `source/qmarkdowntextedit/LICENSE`, MIT, Patrizio Bekerle |
| lzokay | `source/lzokay/LICENSE`, MIT, Jack Andersen |
| maddy | `source/maddy/LICENSE`, permissive MIT-style text, M. Petra Baranski |
| minimp3 | `source/minimp3/LICENSE`, CC0 1.0 |
| heatshrink | `source/heatshrink/LICENSE`, permissive license, Scott Vokes |
| ESP32 bundled support | `source/esp32/LICENSE`, Apache 2.0; inspect individual files for additional terms |
| Roboto | `source/res/fonts/Roboto/LICENSE.txt`, Apache 2.0 |

Qt and deployed Qt modules/plugins require their own applicable notices and licensing review for the actual distribution. This inventory does not establish the license of every transitive dependency.

## Images, icons, and unresolved assets

New MotorLab geometric logo artwork in `source/res/motorlab/` and the two replaced vertical PNGs was drawn for this project. Raster logo lettering uses Arial Bold available in Windows; no Arial font file is added. Review font embedding/distribution terms for any future font bundling.

The retained functional PNG icons under `source/res/icons/` have not been deleted: they are necessary to preserve UI navigation and meaning. A scan of local Markdown/text notices did not identify a reliable per-icon provenance or license. Filename patterns alone are insufficient evidence of origin or permission. These icons therefore remain **unresolved for public redistribution** pending an upstream provenance record or replacement with verified assets. No permissive license is asserted for them.

DejaVu font files and Exan-Regular.ttf are present in the resource inventory. A corresponding font-specific license was not located by this limited review; obtain and retain authoritative license texts before redistribution. Other retained photographs, diagrams, firmware binaries, edition-specific logos, and upstream trademark artwork also need a resource-by-resource review. The GPL source license must not be treated as automatic trademark permission or proof of independent image/font rights.

The neutral desktop resource collection uses the new MotorLab application icon and horizontal logos. Other edition resources remain in the source tree as upstream provenance and must not be selected for MotorLab branding. Do not represent this notice as full legal clearance or publish a binary until unresolved bundled assets and deployment notices are addressed.

## Company artwork supplied by user
Active application branding uses the user-provided Somboon Advance Technology image, stored unchanged as source/res/motorlab/somboon-user-supplied.png. Its use was expressly requested by the user. No ownership, GPL relicensing or independent trademark clearance is asserted. Previous original geometric artwork remains only as historical project material. Qt LGPLv3 and GPLv3 texts retained under licenses/; the local trial deploys matching Qt 5.15.2 DLLs dynamically. Public binary distribution remains pending the documented resource review.

## SAT Motor Pilot 0.2.0 company product
The active product is SAT Motor Pilot. Earlier MotorLab references in this inventory describe preliminary history only. Product UI, onboarding, help, app identity and distribution are company-specific; the GPL-derived compatibility engine keeps all existing copyright notices. The logo remains user-supplied company artwork, without a claim of GPL relicensing. See docs/COMPANY_PRODUCT.md for boundaries.

## 0.3.0 transparent company artwork
The original user-supplied image remains unchanged. Active PNG/ICO branding is a transparent derivative made with image editing, with layout variants generated into existing resource paths. This does not establish new ownership or alter the company artwork's trademark status. The SAT shell uses text navigation instead of the previous navigation raster icons; retained compatibility tools may still use upstream resources subject to the review above.
