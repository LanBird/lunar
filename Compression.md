# Introduction #

Lunar uses deflate compression and the gzip file-format for transmission.

While it is obvious why we use compression you might wonder why the method of choice is gzip. In fact the first idea was to incorporate lzo into lunar because of it's fast (de)compression.

However, since lzo is GPL-licensed I looked for another solution. Writing an lzo-compatible BSD-licensed compressor and decompressor seemed difficult due to the lack of detailed documentation, the obfuscated source code and the mere amount of different algorithms.

So why use gzip then? Gzip is:
  * Well described in [RFC1951](http://tools.ietf.org/html/rfc1951) and [RFC1952](http://tools.ietf.org/html/rfc1952)
  * Widely used
  * Used as compression in HTTP/1.1

Especially the last point is interesting. The ability to communicate as a (simple) HTTP server gives a lot of possibilities. (I won't list them here ..)
Using the HTTP protocol saves us the pain to create yet another proprietary protocol that nobody needs and gives a well-defined interface for third party applications.

Which parts of HTTP/1.1 will be implemented and which parts (on top) are lunar specific can be read [here](Protocol.md).

One more point is that users may want to generate distribution packages. These are most often in the .tar.gz format. We would not want to integrate a full implemetation of tar but it could be interesting to be able to examine package contents and the modification times of files ..