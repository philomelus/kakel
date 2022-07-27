kakel v0.1.23.676.a.45.k

This is a tile slide puzzle game made with Godot.

I originally created this to get familiar with Godot.  Its nothing special, but
it does work and can be fun to play.  Once or twice, anyway.

There are multiple versions of this available in the git repo on github:

master - Follows the 3.x branch of Godot
mono - Follows the 3.x brnach of Godot, using mono
4.x - Follows the master branch of Godot
mono-4.x - Follows the master branch of Godot, using mono

I try to keep all versions in sync, as far as functionality goes.  However, if
you play one version and its different than another version, then most likely
I am in between updating which ever version is older.

I regularly build (export) these projects for Windows 10+, and Android.  I
also build them for Linux (Debian) fairly often as well.  I do not build for,
nor do I test on, nor do I plan to support, Mac or iOS.

Finally, I am also planning on making c++ versions of this as well, but the
status of the Godot 4.x-alpha is very unclear, so it may be a bit before I
get around to it.  As for the 3.x GDNative, I am still thinking it through
(not sure how useful it will be, with the significant changes in 4.x).  If
you are looking for a c++ version, please let me know, as I will be motivated
to pursue it if something other than my own curiousity is available.

Just a note on c++;  I have been programming in c++ since the c-front days.
That was when c-front was used on c++ sources to convert them into c code,
so the native compilers could be used (i.e. no c++ specific compilers
existed yet).  I say this only to note that my hesitantcy on the c++ versions
of Kakel are not because I don't have the skill, but because I see the changes
that are coming in 4.x, and I'm not 100% sure I want to use GDNative, rather
than just making my own module (which will be most performant anyway).  I've
taken on the work involved in keeping the 4 versions I currently have in sync,
and I'm not sure I want to add that much more for something that I'm thinking
will have little to no benefit.  4.x GDNative is unclear to me at this point,
but if its going to have support, then writing this as a module is not really
the correct solution;  But I don't know that yet, so I am waiting to see what
happens with 4.x and GDNative.  Too much uncertianty for the correct way
forward, so I procrastinate until it becomes more clear.  You know, the
usual :-)

--> Russ Gibson
    2022-07-27

