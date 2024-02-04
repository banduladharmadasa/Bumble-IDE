#pragma once
#include "stdafx.h"
#include <map>


static TCHAR *szExtensions[] =
{
	L"abap",
	L"abc",
	L"as",
	L"ada|adb",
	L"^htaccess|^htgroups|^htpasswd|^conf|htaccess|htgroups|htpasswd",
	L"asciidoc|adoc",
	L"dsl|asl",
	L"asm|a",
	L"ahk",
	L"apex|cls|trigger|tgr",
	L"bat|cmd",
	L"bro",
	L"cpp|c|cc|cxx|h|hh|hpp|ino",
	L"c9search_results",
	L"cirru|cr",
	L"clj|cljs",
	L"CBL|COB",
	L"coffee|cf|cson|^Cakefile",
	L"cfm",
	L"cs",
	L"csd",
	L"orc",
	L"sco",
	L"css",
	L"curly",
	L"d|di",
	L"dart",
	L"diff|patch",
	L"^Dockerfile",
	L"dot",
	L"drl",
	L"edi",
	L"e|ge",
	L"ejs",
	L"ex|exs",
	L"elm",
	L"erl|hrl",
	L"frt|fs|ldr|fth|4th",
	L"f|f90",
	L"fsi|fs|ml|mli|fsx|fsscript",
	L"fsl",
	L"ftl",
	L"gcode",
	L"feature",
	L"^.gitignore",
	L"glsl|frag|vert",
	L"gbs",
	L"go",
	L"gql",
	L"groovy",
	L"haml",
	L"hbs|handlebars|tpl|mustache",
	L"hs",
	L"cabal",
	L"hx",
	L"hjson",
	L"html|htm|xhtml|vue|we|wpy",
	L"eex|html.eex",
	L"erb|rhtml|html.erb",
	L"ini|conf|cfg|prefs",
	L"io",
	L"jack",
	L"jade|pug",
	L"java",
	L"js|jsm|jsx",
	L"json",
	L"jq",
	L"jsp",
	L"jssm|jssm_state",
	L"jsx",
	L"jl",
	L"kt|kts",
	L"tex|latex|ltx|bib",
	L"less",
	L"liquid",
	L"lisp",
	L"ls",
	L"logic|lql",
	L"lsl",
	L"lua",
	L"lp",
	L"lucene",
	L"^Makefile|^GNUmakefile|^makefile|^OCamlMakefile|make",
	L"md|markdown",
	L"mask",
	L"matlab",
	L"mz",
	L"mel",
	L"mixal",
	L"mc|mush",
	L"mysql",
	L"nix",
	L"nsi|nsh",
	L"m|mm",
	L"ml|mli",
	L"pas|p",
	L"pl|pm",
	L"p6|pl6|pm6",
	L"pgsql",
	L"blade.php",
	L"php|inc|phtml|shtml|php3|php4|php5|phps|phpt|aw|ctp|module",
	L"epp|pp",
	L"pig",
	L"ps1",
	L"praat|praatscript|psc|proc",
	L"plg|prolog",
	L"properties",
	L"proto",
	L"py",
	L"r",
	L"cshtml|asp",
	L"Rd",
	L"red|reds",
	L"Rhtml",
	L"rst",
	L"rb|ru|gemspec|rake|^Guardfile|^Rakefile|^Gemfile",
	L"rs",
	L"sass",
	L"scad",
	L"scala",
	L"scm|sm|rkt|oak|scheme",
	L"scss",
	L"sh|bash|^.bashrc",
	L"sjs",
	L"slim|skim",
	L"smarty|tpl",
	L"snippets",
	L"soy",
	L"space",
	L"sql",
	L"sqlserver",
	L"styl|stylus",
	L"svg",
	L"swift",
	L"tcl",
	L"tf |tfvars | terragrunt",
	L"tex",
	L"txt",
	L"textile",
	L"toml",
	L"tsx",
	L"latte|twig|swig",
	L"ts|typescript|str",
	L"vala",
	L"vbs|vb",
	L"vm",
	L"v|vh|sv|svh",
	L"vhd|vhdl",
	L"vfp|component|page",
	L"wlk|wpgm|wtest",
	L"xml|rdf|rss|wsdl|xslt|atom|mathml|mml|xul|xbl|xaml",
	L"xq",
	L"yaml|yml",
	NULL,
};





#define TEXT_EDITOR_THEME_SIZE 37
static TCHAR* lpszTextEditorThemes[] =
{
	L"Ambiance",
	L"Chaos",
	L"Chrome",
	L"Clouds",
	L"Clouds_midnight",
	L"Cobalt",
	L"Crimson_editor",
	L"Dawn",
	L"Dracula",
	L"Dreamweaver",
	L"Eclipse",
	L"Github",
	L"Gob",
	L"Gruvbox",
	L"Idle_fingers",
	L"Iplastic",
	L"Katzenmilch",
	L"Kr_theme",
	L"Kuroir",
	L"Merbivore",
	L"Merbivore_soft",
	L"Mono_industrial",
	L"Monokai",
	L"Pastel_on_dark",
	L"Solarized_dark",
	L"Solarized_light",
	L"Sqlserver",
	L"Terminal",
	L"Textmate",
	L"Tomorrow",
	L"Tomorrow_night",
	L"Tomorrow_night_blue",
	L"Tomorrow_night_bright",
	L"Tomorrow_night_eighties",
	L"Twilight",
	L"Vibrant_ink",
	L"Xcode",
NULL
};



static std::map<CString, CString> languageMap;

static void InitLanguageMap()
{
	languageMap[L"Wollok"] = L"wollok";
	languageMap[L"Soy Template"] = L"soy_template";
	languageMap[L"SH"] = L"sh";
	languageMap[L"PHP"] = L"php";
	languageMap[L"Liquid"] = L"liquid";
	languageMap[L"Hjson"] = L"hjson";
	languageMap[L"ActionScript"] = L"actionscript";
	languageMap[L"RHTML"] = L"rhtml";
	languageMap[L"R"] = L"r";
	languageMap[L"Maze"] = L"maze";
	languageMap[L"LaTeX"] = L"latex"; 
	languageMap[L"Jade"] = L"jade";
	languageMap[L"haXe"] = L"haxe";
	languageMap[L"Handlebars"] = L"handlebars";
	languageMap[L"D"] = L"d";
	languageMap[L"Curly"] = L"curly";
	languageMap[L"VHDL"] = L"vhdl";
	languageMap[L"Text"] = L"text";
	languageMap[L"Smarty"] = L"smarty";
	languageMap[L"SJS"] = L"sjs";
	languageMap[L"RDoc"] = L"rdoc";
	languageMap[L"JSSM"] = L"jssm";
	languageMap[L"HTML (Ruby)"] = L"html_ruby";
	languageMap[L"Forth"] = L"forth";
	languageMap[L"EJS"] = L"ejs";
	languageMap[L"Dockerfile"] = L"dockerfile";
	languageMap[L"Csound Score"] = L"csound_score";
	languageMap[L"Clojure"] = L"clojure";
	languageMap[L"XML"] = L"xml";
	languageMap[L"Typescript"] = L"typescript";
	languageMap[L"snippets"] = L"snippets";
	languageMap[L"Slim"] = L"slim";
	languageMap[L"pgSQL"] = L"pgsql";
	languageMap[L"Makefile"] = L"makefile";
	languageMap[L"Kotlin"] = L"kotlin";
	languageMap[L"FSharp"] = L"fsharp";
	languageMap[L"Csound"] = L"csound_orchestra";
	languageMap[L"C#"] = L"csharp";
	languageMap[L"CoffeeScript"] = L"coffee";
	languageMap[L"Pascal"] = L"pascal";
	languageMap[L"Haskell Cabal"] = L"haskell_cabal";
	languageMap[L"Glsl"] = L"glsl";
	languageMap[L"Elm"] = L"elm";
	languageMap[L"ColdFusion"] = L"coldfusion";
	languageMap[L"AsciiDoc"] = L"asciidoc";
	languageMap[L"VBScript"] = L"vbscript";
	languageMap[L"SCAD"] = L"scad";
	languageMap[L"Ruby"] = L"ruby";
	languageMap[L"MATLAB"] = L"matlab";
	languageMap[L"Io"] = L"io";
	languageMap[L"HTML (Elixir)"] = L"html_elixir";
	languageMap[L"Gobstones"] = L"gobstones";
	languageMap[L"Gitignore"] = L"gitignore";
	languageMap[L"Gcode"] = L"gcode";
	languageMap[L"Dot"] = L"dot";
	languageMap[L"Django"] = L"django";
	languageMap[L"Terraform"] = L"terraform";
	languageMap[L"Julia"] = L"julia";
	languageMap[L"INI"] = L"ini";
	languageMap[L"Verilog"] = L"verilog";
	languageMap[L"SCSS"] = L"scss";
	languageMap[L"PHP (Blade Template)"] = L"php_laravel_blade";
	languageMap[L"Objective-C"] = L"objectivec";
	languageMap[L"MIXAL"] = L"mixal";
	languageMap[L"LogiQL"] = L"logiql";
	languageMap[L"Lisp"] = L"lisp";
	languageMap[L"Visualforce"] = L"visualforce";
	languageMap[L"Stylus"] = L"stylus";
	languageMap[L"SQL"] = L"sql";
	languageMap[L"Protobuf"] = L"protobuf";
	languageMap[L"Mask"] = L"mask";
	languageMap[L"Jack"] = L"jack";
	languageMap[L"GraphQLSchema"] = L"graphqlschema";
	languageMap[L"Gherkin"] = L"gherkin";
	languageMap[L"Edifact"] = L"edifact";
	languageMap[L"Csound Document"] = L"csound_document";
	languageMap[L"Apex"] = L"apex";
	languageMap[L"Assembly x86"] = L"assembly_x86";
	languageMap[L"ABAP"] = L"abap";
	languageMap[L"YAML"] = L"yaml";
	languageMap[L"Tcl"] = L"tcl";
	languageMap[L"Rust"] = L"rust";
	languageMap[L"Puppet"] = L"puppet";
	languageMap[L"Perl 6"] = L"perl6";
	languageMap[L"Perl"] = L"perl";
	languageMap[L"Markdown"] = L"markdown";
	languageMap[L"LESS"] = L"less";
	languageMap[L"HAML"] = L"haml";
	languageMap[L"Groovy"] = L"groovy";
	languageMap[L"Cirru"] = L"cirru";
	languageMap[L"C and C++"] = L"c_cpp";
	languageMap[L"TSX"] = L"tsx";
	languageMap[L"Textile"] = L"textile";
	languageMap[L"RST"] = L"rst";
	languageMap[L"Lucene"] = L"lucene";
	languageMap[L"LSL"] = L"lsl";
	languageMap[L"JSX"] = L"jsx";
	languageMap[L"JSP"] = L"jsp";
	languageMap[L"FSL"] = L"fsl";
	languageMap[L"Erlang"] = L"erlang";
	languageMap[L"CSS"] = L"css";
	languageMap[L"Cobol"] = L"cobol";
	languageMap[L"ASL"] = L"asl";
	languageMap[L"ABC"] = L"abc";
	languageMap[L"Toml"] = L"toml";
	languageMap[L"Tex"] = L"tex";
	languageMap[L"Red"] = L"red";
	languageMap[L"Python"] = L"python";
	languageMap[L"MEL"] = L"mel";
	languageMap[L"LuaPage"] = L"luapage";
	languageMap[L"JSONiq"] = L"jsoniq";
	languageMap[L"Go"] = L"golang";
	languageMap[L"C9Search"] = L"c9search";
	languageMap[L"Bro"] = L"bro";
	languageMap[L"AutoHotkey / AutoIt"] = L"autohotkey";
	languageMap[L"XQuery"] = L"xquery";
	languageMap[L"Swift"] = L"swift";
	languageMap[L"Razor"] = L"razor";
	languageMap[L"Properties"] = L"properties";
	languageMap[L"Prolog"] = L"prolog";
	languageMap[L"Lua"] = L"lua";
	languageMap[L"JavaScript"] = L"javascript";
	languageMap[L"Eiffel"] = L"eiffel";
	languageMap[L"Diff"] = L"diff";
	languageMap[L"Apache Conf"] = L"apache_conf";
	languageMap[L"ADA"] = L"ada";
	languageMap[L"Twig"] = L"twig";
	languageMap[L"Praat"] = L"praat";
	languageMap[L"NSIS"] = L"nsis";
	languageMap[L"FreeMarker"] = L"ftl";
	languageMap[L"Vala"] = L"vala";
	languageMap[L"Space"] = L"space";
	languageMap[L"Scala"] = L"scala";
	languageMap[L"Java"] = L"java";
	languageMap[L"HTML"] = L"html";
	languageMap[L"Velocity"] = L"velocity";
	languageMap[L"SVG"] = L"svg";
	languageMap[L"SQLServer"] = L"sqlserver";
	languageMap[L"Scheme"] = L"scheme";
	languageMap[L"SASS"] = L"sass";
	languageMap[L"Powershell"] = L"powershell";
	languageMap[L"OCaml"] = L"ocaml";
	languageMap[L"MySQL"] = L"mysql";
	languageMap[L"LiveScript"] = L"livescript";
	languageMap[L"Elixir"] = L"elixir";
	languageMap[L"Drools"] = L"drools";
	languageMap[L"Pig"] = L"pig";
	languageMap[L"Nix"] = L"nix";
	languageMap[L"MUSHCode"] = L"mushcode";
	languageMap[L"JSON"] = L"json";
	languageMap[L"Haskell"] = L"haskell";
	languageMap[L"Fortran"] = L"fortran";
	languageMap[L"Dart"] = L"dart";
	languageMap[L"BatchFile"] = L"batchfile";

	
}

static TCHAR BASED_CODE szFilter[] =
_T("All Files (*.*)|*.*|")
_T("ABAP (*.abap) | *.abap|")
_T("ABC (*.abc) | *.abc|")
_T("ActionScript (*.as) | *.as|")
_T("ADA (*.ada;*.adb) | *.ada;*.adb|")
_T("Apache Conf (*.^htaccess;*.^htgroups;*.^htpasswd;*.^conf;*.htaccess;*.htgroups;*.htpasswd) | *.^htaccess;*.^htgroups;*.^htpasswd;*.^conf;*.htaccess;*.htgroups;*.htpasswd|")
_T("AsciiDoc (*.asciidoc;*.adoc) | *.asciidoc;*.adoc|")
_T("ASL (*.dsl;*.asl) | *.dsl;*.asl|")
_T("Assembly x86 (*.asm;*.a) | *.asm;*.a|")
_T("AutoHotkey / AutoIt (*.ahk) | *.ahk|")
_T("Apex (*.apex;*.cls;*.trigger;*.tgr) | *.apex;*.cls;*.trigger;*.tgr|")
_T("BatchFile (*.bat;*.cmd) | *.bat;*.cmd|")
_T("Bro (*.bro) | *.bro|")
_T("C and C++ (*.cpp;*.c;*.cc;*.cxx;*.h;*.hh;*.hpp;*.ino) | *.cpp;*.c;*.cc;*.cxx;*.h;*.hh;*.hpp;*.ino|")
_T("C9Search (*.c9search_results) | *.c9search_results|")
_T("Cirru (*.cirru;*.cr) | *.cirru;*.cr|")
_T("Clojure (*.clj;*.cljs) | *.clj;*.cljs|")
_T("Cobol (*.CBL;*.COB) | *.CBL;*.COB|")
_T("CoffeeScript (*.coffee;*.cf;*.cson;*.^Cakefile) | *.coffee;*.cf;*.cson;*.^Cakefile|")
_T("ColdFusion (*.cfm) | *.cfm|")
_T("C# (*.cs) | *.cs|")
_T("Csound Document (*.csd) | *.csd|")
_T("Csound (*.orc) | *.orc|")
_T("Csound Score (*.sco) | *.sco|")
_T("CSS (*.css) | *.css|")
_T("Curly (*.curly) | *.curly|")
_T("D (*.d;*.di) | *.d;*.di|")
_T("Dart (*.dart) | *.dart|")
_T("Diff (*.diff;*.patch) | *.diff;*.patch|")
_T("Dockerfile (*.^Dockerfile) | *.^Dockerfile|")
_T("Dot (*.dot) | *.dot|")
_T("Drools (*.drl) | *.drl|")
_T("Edifact (*.edi) | *.edi|")
_T("Eiffel (*.e;*.ge) | *.e;*.ge|")
_T("EJS (*.ejs) | *.ejs|")
_T("Elixir (*.ex;*.exs) | *.ex;*.exs|")
_T("Elm (*.elm) | *.elm|")
_T("Erlang (*.erl;*.hrl) | *.erl;*.hrl|")
_T("Forth (*.frt;*.fs;*.ldr;*.fth;*.4th) | *.frt;*.fs;*.ldr;*.fth;*.4th|")
_T("Fortran (*.f;*.f90) | *.f;*.f90|")
_T("FSharp (*.fsi;*.fs;*.ml;*.mli;*.fsx;*.fsscript) | *.fsi;*.fs;*.ml;*.mli;*.fsx;*.fsscript|")
_T("FSL (*.fsl) | *.fsl|")
_T("FreeMarker (*.ftl) | *.ftl|")
_T("Gcode (*.gcode) | *.gcode|")
_T("Gherkin (*.feature) | *.feature|")
_T("Gitignore (*.^.gitignore) | *.^.gitignore|")
_T("Glsl (*.glsl;*.frag;*.vert) | *.glsl;*.frag;*.vert|")
_T("Gobstones (*.gbs) | *.gbs|")
_T("Go (*.go) | *.go|")
_T("GraphQLSchema (*.gql) | *.gql|")
_T("Groovy (*.groovy) | *.groovy|")
_T("HAML (*.haml) | *.haml|")
_T("Handlebars (*.hbs;*.handlebars;*.tpl;*.mustache) | *.hbs;*.handlebars;*.tpl;*.mustache|")
_T("Haskell (*.hs) | *.hs|")
_T("Haskell Cabal (*.cabal) | *.cabal|")
_T("haXe (*.hx) | *.hx|")
_T("Hjson (*.hjson) | *.hjson|")
_T("HTML (*.html;*.htm;*.xhtml;*.vue;*.we;*.wpy) | *.html;*.htm;*.xhtml;*.vue;*.we;*.wpy|")
_T("HTML (Elixir) (*.eex;*.html.eex) | *.eex;*.html.eex|")
_T("HTML (Ruby) (*.erb;*.rhtml;*.html.erb) | *.erb;*.rhtml;*.html.erb|")
_T("INI (*.ini;*.conf;*.cfg;*.prefs) | *.ini;*.conf;*.cfg;*.prefs|")
_T("Io (*.io) | *.io|")
_T("Jack (*.jack) | *.jack|")
_T("Jade (*.jade;*.pug) | *.jade;*.pug|")
_T("Java (*.java) | *.java|")
_T("JavaScript (*.js;*.jsm;*.jsx) | *.js;*.jsm;*.jsx|")
_T("JSON (*.json) | *.json|")
_T("JSONiq (*.jq) | *.jq|")
_T("JSP (*.jsp) | *.jsp|")
_T("JSSM (*.jssm;*.jssm_state) | *.jssm;*.jssm_state|")
_T("JSX (*.jsx) | *.jsx|")
_T("Julia (*.jl) | *.jl|")
_T("Kotlin (*.kt;*.kts) | *.kt;*.kts|")
_T("LaTeX (*.tex;*.latex;*.ltx;*.bib) | *.tex;*.latex;*.ltx;*.bib|")
_T("LESS (*.less) | *.less|")
_T("Liquid (*.liquid) | *.liquid|")
_T("Lisp (*.lisp) | *.lisp|")
_T("LiveScript (*.ls) | *.ls|")
_T("LogiQL (*.logic;*.lql) | *.logic;*.lql|")
_T("LSL (*.lsl) | *.lsl|")
_T("Lua (*.lua) | *.lua|")
_T("LuaPage (*.lp) | *.lp|")
_T("Lucene (*.lucene) | *.lucene|")
_T("Makefile (*.^Makefile;*.^GNUmakefile;*.^makefile;*.^OCamlMakefile;*.make) | *.^Makefile;*.^GNUmakefile;*.^makefile;*.^OCamlMakefile;*.make|")
_T("Markdown (*.md;*.markdown) | *.md;*.markdown|")
_T("Mask (*.mask) | *.mask|")
_T("MATLAB (*.matlab) | *.matlab|")
_T("Maze (*.mz) | *.mz|")
_T("MEL (*.mel) | *.mel|")
_T("MIXAL (*.mixal) | *.mixal|")
_T("MUSHCode (*.mc;*.mush) | *.mc;*.mush|")
_T("MySQL (*.mysql) | *.mysql|")
_T("Nix (*.nix) | *.nix|")
_T("NSIS (*.nsi;*.nsh) | *.nsi;*.nsh|")
_T("Objective-C (*.m;*.mm) | *.m;*.mm|")
_T("OCaml (*.ml;*.mli) | *.ml;*.mli|")
_T("Pascal (*.pas;*.p) | *.pas;*.p|")
_T("Perl (*.pl;*.pm) | *.pl;*.pm|")
_T("Perl 6 (*.p6;*.pl6;*.pm6) | *.p6;*.pl6;*.pm6|")
_T("pgSQL (*.pgsql) | *.pgsql|")
_T("PHP (Blade Template) (*.blade.php) | *.blade.php|")
_T("PHP (*.php;*.inc;*.phtml;*.shtml;*.php3;*.php4;*.php5;*.phps;*.phpt;*.aw;*.ctp;*.module) | *.php;*.inc;*.phtml;*.shtml;*.php3;*.php4;*.php5;*.phps;*.phpt;*.aw;*.ctp;*.module|")
_T("Puppet (*.epp;*.pp) | *.epp;*.pp|")
_T("Pig (*.pig) | *.pig|")
_T("Powershell (*.ps1) | *.ps1|")
_T("Praat (*.praat;*.praatscript;*.psc;*.proc) | *.praat;*.praatscript;*.psc;*.proc|")
_T("Prolog (*.plg;*.prolog) | *.plg;*.prolog|")
_T("Properties (*.properties) | *.properties|")
_T("Protobuf (*.proto) | *.proto|")
_T("Python (*.py) | *.py|")
_T("R (*.r) | *.r|")
_T("Razor (*.cshtml;*.asp) | *.cshtml;*.asp|")
_T("RDoc (*.Rd) | *.Rd|")
_T("Red (*.red;*.reds) | *.red;*.reds|")
_T("RHTML (*.Rhtml) | *.Rhtml|")
_T("RST (*.rst) | *.rst|")
_T("Ruby (*.rb;*.ru;*.gemspec;*.rake;*.^Guardfile;*.^Rakefile;*.^Gemfile) | *.rb;*.ru;*.gemspec;*.rake;*.^Guardfile;*.^Rakefile;*.^Gemfile|")
_T("Rust (*.rs) | *.rs|")
_T("SASS (*.sass) | *.sass|")
_T("SCAD (*.scad) | *.scad|")
_T("Scala (*.scala) | *.scala|")
_T("Scheme (*.scm;*.sm;*.rkt;*.oak;*.scheme) | *.scm;*.sm;*.rkt;*.oak;*.scheme|")
_T("SCSS (*.scss) | *.scss|")
_T("SH (*.sh;*.bash;*.^.bashrc) | *.sh;*.bash;*.^.bashrc|")
_T("SJS (*.sjs) | *.sjs|")
_T("Slim (*.slim;*.skim) | *.slim;*.skim|")
_T("Smarty (*.smarty;*.tpl) | *.smarty;*.tpl|")
_T("snippets (*.snippets) | *.snippets|")
_T("Soy Template (*.soy) | *.soy|")
_T("Space (*.space) | *.space|")
_T("SQL (*.sql) | *.sql|")
_T("SQLServer (*.sqlserver) | *.sqlserver|")
_T("Stylus (*.styl;*.stylus) | *.styl;*.stylus|")
_T("SVG (*.svg) | *.svg|")
_T("Swift (*.swift) | *.swift|")
_T("Tcl (*.tcl) | *.tcl|")
_T("Terraform (*.tf ;*.tfvars ;*. terragrunt) | *.tf ;*.tfvars ;*. terragrunt|")
_T("Tex (*.tex) | *.tex|")
_T("Text (*.txt) | *.txt|")
_T("Textile (*.textile) | *.textile|")
_T("Toml (*.toml) | *.toml|")
_T("TSX (*.tsx) | *.tsx|")
_T("Twig (*.latte;*.twig;*.swig) | *.latte;*.twig;*.swig|")
_T("Typescript (*.ts;*.typescript;*.str) | *.ts;*.typescript;*.str|")
_T("Vala (*.vala) | *.vala|")
_T("VBScript (*.vbs;*.vb) | *.vbs;*.vb|")
_T("Velocity (*.vm) | *.vm|")
_T("Verilog (*.v;*.vh;*.sv;*.svh) | *.v;*.vh;*.sv;*.svh|")
_T("VHDL (*.vhd;*.vhdl) | *.vhd;*.vhdl|")
_T("Visualforce (*.vfp;*.component;*.page) | *.vfp;*.component;*.page|")
_T("Wollok (*.wlk;*.wpgm;*.wtest) | *.wlk;*.wpgm;*.wtest|")
_T("XML (*.xml;*.rdf;*.rss;*.wsdl;*.xslt;*.atom;*.mathml;*.mml;*.xul;*.xbl;*.xaml) | *.xml;*.rdf;*.rss;*.wsdl;*.xslt;*.atom;*.mathml;*.mml;*.xul;*.xbl;*.xaml|")
_T("XQuery (*.xq) | *.xq|")
_T("YAML (*.yaml;*.yml) | *.yaml;*.yml||");



CString GetHTMLColorString(COLORREF color);
CString GetRGBColorString(COLORREF color);
CString GetHSVColorString(COLORREF color);
CString GetHSLColorString(COLORREF color);
CString GetCMYKColorString(COLORREF color);



enum AppMessageType
{
	AppMessageType_Info = 0,
	AppMessageType_Error,
	AppMessageType_Warning,
	AppMessageType_Character,
	AppMessageTpe_ShellOutput,
};

static std::map<CString, int> extToIconId;
static std::map<CString, CString> langToExtensionMap;
static void InitLangToExtensionMap()
{
	langToExtensionMap[L"ABAP"] = L"abap";
	langToExtensionMap[L"ABC"] = L"abc";
	langToExtensionMap[L"ActionScript"] = L"as";
	langToExtensionMap[L"ADA"] = L"ada|adb";
	langToExtensionMap[L"Apache_Conf"] = L"^htaccess|^htgroups|^htpasswd|^conf|htaccess|htgroups|htpasswd";
	langToExtensionMap[L"AsciiDoc"] = L"asciidoc|adoc";
	langToExtensionMap[L"ASL"] = L"dsl|asl";
	langToExtensionMap[L"Assembly_x86"] = L"asm|a";
	langToExtensionMap[L"AutoHotKey"] = L"ahk";
	langToExtensionMap[L"Apex"] = L"apex|cls|trigger|tgr";
	langToExtensionMap[L"AQL"] = L"aql";
	langToExtensionMap[L"BatchFile"] = L"bat|cmd";
	langToExtensionMap[L"C_Cpp"] = L"cpp|c|cc|cxx|h|hh|hpp|ino";
	langToExtensionMap[L"C9Search"] = L"c9search_results";
	langToExtensionMap[L"Crystal"] = L"cr";
	langToExtensionMap[L"Cirru"] = L"cirru|cr";
	langToExtensionMap[L"Clojure"] = L"clj|cljs";
	langToExtensionMap[L"Cobol"] = L"CBL|COB";
	langToExtensionMap[L"coffee"] = L"coffee|cf|cson|^Cakefile";
	langToExtensionMap[L"ColdFusion"] = L"cfm";
	langToExtensionMap[L"CSharp"] = L"cs";
	langToExtensionMap[L"Csound_Document"] = L"csd";
	langToExtensionMap[L"Csound_Orchestra"] = L"orc";
	langToExtensionMap[L"Csound_Score"] = L"sco";
	langToExtensionMap[L"CSS"] = L"css";
	langToExtensionMap[L"Curly"] = L"curly";
	langToExtensionMap[L"D"] = L"d|di";
	langToExtensionMap[L"Dart"] = L"dart";
	langToExtensionMap[L"Diff"] = L"diff|patch";
	langToExtensionMap[L"Dockerfile"] = L"^Dockerfile";
	langToExtensionMap[L"Dot"] = L"dot";
	langToExtensionMap[L"Drools"] = L"drl";
	langToExtensionMap[L"Edifact"] = L"edi";
	langToExtensionMap[L"Eiffel"] = L"e|ge";
	langToExtensionMap[L"EJS"] = L"ejs";
	langToExtensionMap[L"Elixir"] = L"ex|exs";
	langToExtensionMap[L"Elm"] = L"elm";
	langToExtensionMap[L"Erlang"] = L"erl|hrl";
	langToExtensionMap[L"Forth"] = L"frt|fs|ldr|fth|4th";
	langToExtensionMap[L"Fortran"] = L"f|f90";
	langToExtensionMap[L"FSharp"] = L"fsi|fs|ml|mli|fsx|fsscript";
	langToExtensionMap[L"FSL"] = L"fsl";
	langToExtensionMap[L"FTL"] = L"ftl";
	langToExtensionMap[L"Gcode"] = L"gcode";
	langToExtensionMap[L"Gherkin"] = L"feature";
	langToExtensionMap[L"Gitignore"] = L"^.gitignore";
	langToExtensionMap[L"Glsl"] = L"glsl|frag|vert";
	langToExtensionMap[L"Gobstones"] = L"gbs";
	langToExtensionMap[L"golang"] = L"go";
	langToExtensionMap[L"GraphQLSchema"] = L"gql";
	langToExtensionMap[L"Groovy"] = L"groovy";
	langToExtensionMap[L"HAML"] = L"haml";
	langToExtensionMap[L"Handlebars"] = L"hbs|handlebars|tpl|mustache";
	langToExtensionMap[L"Haskell"] = L"hs";
	langToExtensionMap[L"Haskell_Cabal"] = L"cabal";
	langToExtensionMap[L"haXe"] = L"hx";
	langToExtensionMap[L"Hjson"] = L"hjson";
	langToExtensionMap[L"HTML"] = L"html|htm|xhtml|vue|we|wpy";
	langToExtensionMap[L"HTML_Elixir"] = L"eex|html.eex";
	langToExtensionMap[L"HTML_Ruby"] = L"erb|rhtml|html.erb";
	langToExtensionMap[L"INI"] = L"ini|conf|cfg|prefs";
	langToExtensionMap[L"Io"] = L"io";
	langToExtensionMap[L"Jack"] = L"jack";
	langToExtensionMap[L"Jade"] = L"jade|pug";
	langToExtensionMap[L"Java"] = L"java";
	langToExtensionMap[L"JavaScript"] = L"js|jsm|jsx";
	langToExtensionMap[L"JSON"] = L"json";
	langToExtensionMap[L"JSONiq"] = L"jq";
	langToExtensionMap[L"JSP"] = L"jsp";
	langToExtensionMap[L"JSSM"] = L"jssm|jssm_state";
	langToExtensionMap[L"JSX"] = L"jsx";
	langToExtensionMap[L"Julia"] = L"jl";
	langToExtensionMap[L"Kotlin"] = L"kt|kts";
	langToExtensionMap[L"LaTeX"] = L"tex|latex|ltx|bib";
	langToExtensionMap[L"LESS"] = L"less";
	langToExtensionMap[L"Liquid"] = L"liquid";
	langToExtensionMap[L"Lisp"] = L"lisp";
	langToExtensionMap[L"LiveScript"] = L"ls";
	langToExtensionMap[L"LogiQL"] = L"logic|lql";
	langToExtensionMap[L"LSL"] = L"lsl";
	langToExtensionMap[L"Lua"] = L"lua";
	langToExtensionMap[L"LuaPage"] = L"lp";
	langToExtensionMap[L"Lucene"] = L"lucene";
	langToExtensionMap[L"Makefile"] = L"^Makefile|^GNUmakefile|^makefile|^OCamlMakefile|make";
	langToExtensionMap[L"Markdown"] = L"md|markdown";
	langToExtensionMap[L"Mask"] = L"mask";
	langToExtensionMap[L"MATLAB"] = L"matlab";
	langToExtensionMap[L"Maze"] = L"mz";
	langToExtensionMap[L"MEL"] = L"mel";
	langToExtensionMap[L"MIXAL"] = L"mixal";
	langToExtensionMap[L"MUSHCode"] = L"mc|mush";
	langToExtensionMap[L"MySQL"] = L"mysql";
	langToExtensionMap[L"Nginx"] = L"nginx|conf";
	langToExtensionMap[L"Nix"] = L"nix";
	langToExtensionMap[L"Nim"] = L"nim";
	langToExtensionMap[L"NSIS"] = L"nsi|nsh";
	langToExtensionMap[L"ObjectiveC"] = L"m|mm";
	langToExtensionMap[L"OCaml"] = L"ml|mli";
	langToExtensionMap[L"Pascal"] = L"pas|p";
	langToExtensionMap[L"Perl"] = L"pl|pm";
	langToExtensionMap[L"Perl6"] = L"p6|pl6|pm6";
	langToExtensionMap[L"pgSQL"] = L"pgsql";
	langToExtensionMap[L"PHP_Laravel_blade"] = L"blade.php";
	langToExtensionMap[L"PHP"] = L"php|inc|phtml|shtml|php3|php4|php5|phps|phpt|aw|ctp|module";
	langToExtensionMap[L"Puppet"] = L"epp|pp";
	langToExtensionMap[L"Pig"] = L"pig";
	langToExtensionMap[L"Powershell"] = L"ps1";
	langToExtensionMap[L"Praat"] = L"praat|praatscript|psc|proc";
	langToExtensionMap[L"Prolog"] = L"plg|prolog";
	langToExtensionMap[L"Properties"] = L"properties";
	langToExtensionMap[L"Protobuf"] = L"proto";
	langToExtensionMap[L"Python"] = L"py";
	langToExtensionMap[L"R"] = L"r";
	langToExtensionMap[L"Razor"] = L"cshtml|asp";
	langToExtensionMap[L"RDoc"] = L"Rd";
	langToExtensionMap[L"Red"] = L"red|reds";
	langToExtensionMap[L"RHTML"] = L"Rhtml";
	langToExtensionMap[L"RST"] = L"rst";
	langToExtensionMap[L"Ruby"] = L"rb|ru|gemspec|rake|^Guardfile|^Rakefile|^Gemfile";
	langToExtensionMap[L"Rust"] = L"rs";
	langToExtensionMap[L"SASS"] = L"sass";
	langToExtensionMap[L"SCAD"] = L"scad";
	langToExtensionMap[L"Scala"] = L"scala|sbt";
	langToExtensionMap[L"Scheme"] = L"scm|sm|rkt|oak|scheme";
	langToExtensionMap[L"SCSS"] = L"scss";
	langToExtensionMap[L"SH"] = L"sh|bash|^.bashrc";
	langToExtensionMap[L"SJS"] = L"sjs";
	langToExtensionMap[L"Slim"] = L"slim|skim";
	langToExtensionMap[L"Smarty"] = L"smarty|tpl";
	langToExtensionMap[L"snippets"] = L"snippets";
	langToExtensionMap[L"Soy_Template"] = L"soy";
	langToExtensionMap[L"Space"] = L"space";
	langToExtensionMap[L"SQL"] = L"sql";
	langToExtensionMap[L"SQLServer"] = L"sqlserver";
	langToExtensionMap[L"Stylus"] = L"styl|stylus";
	langToExtensionMap[L"SVG"] = L"svg";
	langToExtensionMap[L"Swift"] = L"swift";
	langToExtensionMap[L"Tcl"] = L"tcl";
	langToExtensionMap[L"Terraform"] = L"tf", "tfvars", "terragrunt";
	langToExtensionMap[L"Tex"] = L"tex";
	langToExtensionMap[L"Text"] = L"txt";
	langToExtensionMap[L"Textile"] = L"textile";
	langToExtensionMap[L"Toml"] = L"toml";
	langToExtensionMap[L"TSX"] = L"tsx";
	langToExtensionMap[L"Twig"] = L"latte|twig|swig";
	langToExtensionMap[L"Typescript"] = L"ts|typescript|str";
	langToExtensionMap[L"Vala"] = L"vala";
	langToExtensionMap[L"VBScript"] = L"vbs|vb";
	langToExtensionMap[L"Velocity"] = L"vm";
	langToExtensionMap[L"Verilog"] = L"v|vh|sv|svh";
	langToExtensionMap[L"VHDL"] = L"vhd|vhdl";
	langToExtensionMap[L"Visualforce"] = L"vfp|component|page";
	langToExtensionMap[L"Wollok"] = L"wlk|wpgm|wtest";
	langToExtensionMap[L"XML"] = L"xml|rdf|rss|wsdl|xslt|atom|mathml|mml|xul|xbl|xaml";
	langToExtensionMap[L"XQuery"] = L"xq";
	langToExtensionMap[L"YAML"] = L"yaml|yml";
	langToExtensionMap[L"Zeek"] = L"zeek|bro";
	// Add the missing mode "Django" to ext-modelist
	langToExtensionMap[L"Django"] = L"html";


	//int iconId = 0;
	//for (auto it : langToExtensionMap) {


	//	CString str = it.second;

	//	int nTokenPos = 0;
	//	CString strToken = str.Tokenize(_T("|"), nTokenPos);

	//	while (!strToken.IsEmpty())
	//	{
	//		OutputDebugString(strToken);
	//		OutputDebugString(L" = ");
	//		OutputDebugString(it.first);
	//		OutputDebugString(L"\n");
	//		strToken = str.Tokenize(_T("|"), nTokenPos);
	//	}

	//	
	//}
}