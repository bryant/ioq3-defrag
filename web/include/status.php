<h2>Port Status</h2>
<p>Here is a table of the most up to date information we have regarding icculus.org/Quake3's working status on different operating systems. There are things to bear in mind when viewing this:
</p>
<ul>
	<li>The status column refers only to the latest information we are given, so it may be out of date.</li>
	<li>"Working" doesn't always imply "usable", see the <em>Notes</em> column.</li>
	<li>Status is one of <em>working</em>, <em>broken</em> or <em>unknown</em>.</li>
</ul>
<table>
	<caption>Status of Port Targets</caption>
	<tr>
		<th scope="col"><acronym title="Operating System">OS</acronym></th>
		<th scope="col">Platform</th>
		<th scope="col">Status</th>
		<th scope="col"><acronym title="Last Known Working Revision">LKWR</acronym></th>
		<th scope="col">Notes</th>
	</tr>
	<tr>
		<td scope="row" rowspan="4">Linux</td> <!--OS-->
		<td>gcc-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-x86_64</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>JIT bytecode compiler requires GNU <tt>as</tt>.</td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-ppc</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>JIT compiler does not work.</td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-alpha</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>Dedicated server only with VM interpreter.</td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row" rowspan="2">Windows</td> <!--OS-->
		<td>mingw-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td>MSVC-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row" rowspan="3">Mac OS X</td> <!--OS-->
		<td>gcc-ppcG3</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-ppcG4</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>JIT compiler does not work.</td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row">FreeBSD</td> <!--OS-->
		<td>gcc-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row">NetBSD</td> <!--OS-->
		<td>gcc-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>Dedicated server only with native libs.</td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row" rowspan="2">Solaris</td> <!--OS-->
		<td>gcc-x86</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td>gcc-sparc</td> <!--Platform-->
		<td>working</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td></td> <!--Notes-->
	</tr>
	<tr>
		<td scope="row">IRIX</td> <!--OS-->
		<td>cc-*</td> <!--Platform-->
		<td>broken</td> <!--Status-->
		<td></td> <!--LKWR-->
		<td>Patch pending, I think.</td> <!--Notes-->
	</tr>
</table>
<p>You can keep an eye on the Subversion repository via the <a href="http://svn.icculus.org/quake3/trunk/">
Web interface</a>.</p>

<h2>Implemented Features</h2>
<ul>
	<li><a href="http://www.openal.org/">OpenAL</a> support for sound. Surround
	sound supported.</li>
	<li>AVI video capture of demos.</li>
	<li>Ogg Vorbis support.</li>
	<li><a href="http://libsdl.org" title="Simple DirectMedia Layer">SDL</a> being used
	for input, OpenGL context management, and sound. Though not on Windows yet.</li>
	<li>Colors converted to ANSI escape codes in terminal output.
	(set <tt>ttycon_ansicolor</tt> to 1 to enable this.)</li>
	<li>Much improved autocompletion on the console.</li>
	<li>Persistent console history.</li>
	<li>Much improved QVM tools.</li>
	<li>MinGW compilation support on Windows and cross-compilation on Linux</li>
	<li>More supported platforms, see the table above.</li>
	<li>Lots and lots of bug fixes.</li>
</ul>

 * SDL backend for unix-like operating systems
* OpenAL sound API support (multiple speaker support and better sound
quality)
* Full x86_64 support on Linux
* MinGW compilation support on Windows and cross compilation support on Linux
* AVI video capture of demos
* Much improved console autocompletion
* Persistent console history
* Colorized terminal output
* Optional Ogg Vorbis support
* Much improved QVM tools
* Support for various esoteric operating systems (see
http://icculus.org/quake3/?page=status)
* Many, many bug fixes

<h2>The Future</h2>
<h3>Release Criteria</h3>
<p>Before a release can occur, the following must occur for each platform:</p>
<ol>
	<li>Testing of <em>Quake 3: Arena</em> single- and multi-player.</li>
	<li>Testing of <em>Quake 3: Team Arena</em> single- and multi-player.</li>
	<li>Testing of at least one other Quake 3 modification. Possibly
	<a href="http://www.urbanterror.net/">Urban Terror</a>,
	<a href="http://www.rq3.com/">Reaction Quake 3</a>, or any other popular
	modification that puts the engine through it's paces.</li>
	<li>An installer which includes all up-to-date id <tt>pk3</tt>
	files, requires original CD-ROM. Users will need to copy the
	<tt>pak0.pk3</tt> file themselves.</li>
</ol>

<h3>Roadmap</h3>
<ul>
	<li>
	<h4>For 1.34:</h4>
	<ul>
		<li>Features implemented, ready for release.</li>
		<li>Meet release criteria (see above) for Win32, Linux, and
		Mac OS X.</li>
	</ul>
	</li>
	<li>
	<h4>For 1.35+:</h4>
	<ul>
		<li><a href="http://en.wikipedia.org/wiki/IPv6" title="Internet Protocol version 6">IPv6</a> support.</li>
		<li>Merge <a class="shell" href="http://svn.icculus.org/quake3/trunk/code/unix/unix_net.c?view=markup">
		unix_net.c</a> and <a class="shell" href="http://svn.icculus.org/quake3/trunk/code/win32/win_net.c?view=markup">
		win_net.c</a>.</li>
		<li>Replace Windows DirectX backend with
		<a href="http://libsdl.org" title="Simple DirectMedia Layer">SDL</a>.</li>
	</ul>
	</li>
	<li>
	<h4>For 1.39:</h4>
	<ul>
		<li>Four player split-screen.</li>
	</ul>
	</li>
</ul>

<p><em>Note: The above is a list of steps that <strong>remain to be completed</strong>,
so the lists will get shorter as release approaches. Items already completed are
not on the roadmap.</em></p>

<h2>PunkBuster Support</h2>
<p><a href="http://www.evenbalance.com/">Even Balance</a>'s Punkbuster support can never
be included with any open-source version of Quake 3 due to it being removed from the
source-code before the release, and the binary-only nature of its anti-cheating software.
</p>
<p>While it is possible that someone could write an open-source equivalent of PunkBuster,
it's not very feasible for a number of reasons.</p>
