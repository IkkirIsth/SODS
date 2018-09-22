Old Ship Designer Custom Editor for use in Shores of Hazeron (not officially affiliated with Shores of Hazeron or Software Engineering, Inc.)

Shores Offline Ship Designer  (by Ikkir aka Moghjubar aka @Ikkir_Isth aka KRb)

This is a program I made in order to learn OpenGL (and to a lesser extent SDL 1.3) sometime during 2012 / 2013, when Shores of Hazeron didnt have an offline ship designer and was free to play.
It featured room copy/paste and rotate, as well as floodfill and free-drawing (though the actual Hazeron ship engine didn't support all that it could do at the time, so it was limited in function to comply with game standards)

Notable issues/learned stuff: A lot, really.  Mostly learned a bunch of UI stuff and resolution things, notably that I should deal with multiple resolutions at the start instead of trying to patch it back in later
(would have gone with a virtual resolution for my math stuff)

This designer is no longer supported by actual Shores of Hazeron so is simply presented AS IS, as part of showing the old learning process.  Notable things inside:
point-in-poly and point-on-line stuff are useful in a lot of places
general drawing setup for GL 1.4 (really old version that runs on everything)
various other things in the un-refactored mess.
the start of actual file management (I think thats in this version?), though I didnt get in part saving and re-using, which would have been a next-major-feature.

Requirements: SDL (various parts: SDL and SDL_Image) and GL