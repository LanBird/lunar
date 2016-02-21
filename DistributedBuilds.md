# Introduction #

As a long-term goal we want to support distributed builds. This involves a lot of work, since there is lot to take care of.

This article describes what problems we will face in development, how they are solved and gives a short overview of the current status of maturity.


# Details #

When distributing builds between multiple computers there are a lot of problems that need to be handled. (More content later..)


# Status #

Right now (since Lunar can't even build anything) there is no support for distributed builds.
First versions of build distribution will be able to inform other Lunar instances via network of changes and interchange build targets.

The next step will be optimization of the dependency tree so that only few files need to be sent over network. This requires exchanging information about modification time and checksums.　Up to this point files are not synchronized and the user will need to use NFS or something similar.

At last the file synchronization will be implemented. At this point the most difficult tasks have already been solved and the synchronization is barely compressing a file, sending it to the recipient who will decompress it and use it for building.

# Solution #

Each target specifies input and output files. When triggering a build on a remote host, all input files have to be updated. If the build finishes successfully the remote host is known to have up-to-date versions of the output files.