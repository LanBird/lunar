# Table of Contents #
  * Roadmap
  * Version numbers
    * lunar/alpha
    * lunar/c
    * lunar/ccrt

# Roadmap #

The first releases of lunar will be called [lunar/alpha](LunarAlphaReleases.md)
and is codenamed lunacy.
All alpha development happens in a closed environment. Suggestions can be made
but will likely by on "hold" until the first production release.
This does /not/ mean it will be closed source.

There is no definite deadline for the first release yet, but I will push myself
to give you a first working release soon. Frequent releases of my current work
will be made thereafter.

The development of lunar/alpha aims for a stable functional release so that
developers might find interest in the project.

The first production release has already defined goals, feature requirements,
implementation details and so forth.

After a minimum level of maturity is reached the first production release will
be made and the project will focus on PR to encourage interested developers to
become contributors. This initial release and all following releases will be
named [lunar/c](LunarCReleases.md) (Hence the codename of lunar/alpha).

Development will go on until lunar/c is rock-stable and provides a useful but
not bloated set of core functionality.

Lunar will be used as a build system for the yet to be developed `libccrt`.
At some point `libccrt` will be able to replace the C-implementation of lunar
and all lunar functionality will be implemented on top of `libccrt`.
(This does create a nice hen-egg-problem ..)
All libccrt-based releases will be named [lunar/ccrt](LunarCcrtReleases.md).

# Version numbers #

## lunar/alpha ##

Alpha releases will be version-numbered `0.1`, `0.2`, `0.3` and will be
available as source-code downloads as well as subversion tags.

## lunar/c ##

C-based releases will be numbered `1.0`, `1.1`, `1.2` and will be available as
binary distributions, and source-code downloads as well as subversion tags.

In-between releases will be numbered `1.0.1`, `1.0.2`, `1.0.3` and will be
solely available as subversion tags.

## lunar/ccrt ##

libccrt-based releases will be numbered `2.0`, `2.1`, `2.2` and will be
available as binary distributions, and source-code downloads as well as
subversion tags.

In-between releases will be numbered `2.0.1`, `2.0.2`, `2.0.3` and will be
solely available as subversion tags.