Lunar seeks to become a buildsystem framework that enables it's user to write complex, feature rich buildsystems with ease.

The core of Lunar consists of a file database to track modifications, a dependency calculator, a multithreaded build queue and means to distribute builds between multiple instances of Lunar via network.
In fact the last one is most terrific since files have to be kept synchronized and generated output files have to be published to the other instances with minimal delay to name just a few problems with the idea. More information on the status of build distribution can be found [here](DistributedBuilds.md).

Lunar development has just started. Everyone who has interest in contributing, please wait until I finished the first working version.
(If you **really** want to join in earlier you can contact me via mail.)