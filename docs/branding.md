# MotorLab product identity

MotorLab is a modified build of VESC Tool 7.00, pinned to upstream commit `01d5f10901116c311e3fb84d5a1541f663d3ce20`.

The desktop application title and About title are MotorLab. Qt organization is MotorLab, organization domain is motorlab.local, application name is MotorLab, and Windows AppUserModelID is MotorLab.Desktop. The new settings namespace intentionally keeps MotorLab settings separate from upstream installations.

The neutral resource collection maps the existing icon and logo resource aliases to MotorLab assets in `source/res/motorlab/`. Both vertical logos retain the original image dimensions. The design uses a navy rounded square, mint rotor ring, and a white geometric M. These assets were drawn for this project; logo lettering uses rasterized Arial Bold from the Windows installation, without bundling that font file. New geometric artwork is offered under GPL-3.0-or-later with the modified application. Existing functional icons and screen layouts are retained.

The About dialog identifies this as a modified build, states GPL-3.0-or-later and no warranty, includes the pinned source commit, and retains Benjamin Vedder's copyright, email, and upstream website. Original source copyright headers, LICENSE, protocol identifiers, schema names and hardware behavior are retained. The changelog remains named for VESC Tool because it contains upstream history.

Branding edits do not imply endorsement by upstream. Upstream trademark-bearing resources for other editions remain in the source archive for provenance; the desktop build must select `res_neutral.qrc` only. No claim is made that every retained image or font has been cleared for redistribution: see THIRD_PARTY_NOTICES.md. Before public distribution, resolve the assets listed there and provide complete corresponding source with build instructions and notices.

The welcome heading, mobile window title, and device discovery banner also use MotorLab branding. Both symbol_v_wide PNG resources now contain the MotorLab mark and name at their original canvas sizes. The instrumented reference retains pinned original copies of these banners and the welcome form.

## User-requested company identity, 2026-10-22
The user supplied the Somboon Advance Technology logo and requested its use plus a company blue theme. The original image is retained unchanged at source/res/motorlab/somboon-user-supplied.png. It is scaled/padded for application slots without redrawing the mark. This is user-supplied trademark artwork, not newly created GPL artwork or a claim of company endorsement. Main selection and Material accents use #304B8A. Operational alert colors and chart series retain their meanings. Earlier geometric MotorLab design is superseded for the active build. The reference screenshots retain original upstream branding; intentional company identity/theme differences are included in pixel metrics.
