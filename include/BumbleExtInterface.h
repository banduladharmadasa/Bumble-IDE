#ifndef HLEDTPLUGINBASE_H
#define  HLEDTPLUGINBASE_H

#ifdef HLEDTPLUGIN_EXPORTS
#define BUMBLE_EXT_API __declspec(dllexport)
#else
#define BUMBLE_EXT_API __declspec(dllimport)
#endif


#include <vector>
#include <memory>
#include "resource.h"


namespace BumbleIDE {


	#define BumbleManageState()  AFX_MANAGE_STATE(AfxGetStaticModuleState())
	

	/// <summary>
	/// Bumble IDE reserves the command values from 0x7B00 to 0x7CFF for internal usage. With your extension,
	/// you communicate with the active document using these values as commands in conjunction with
	/// NotifyDocument() and OnNotify() functions. The communication between the active document and the
	/// extension is bi-directional but none-overlapped (synchronous). Bumble IDE has defined a naming 
	/// convention when labeling these constant integral values. According to this convention, IN_ prefixes
	/// all the values making a communication channel from the extension to the active document, and OUT_ 
	/// prefixes the remaining values to denote the values in the opposite direction of communication.
	/// </summary>
	#define IN_BUMBLE_CONTENT_READY		0x7B00
	#define IN_BUMBLE_CONTENT_APPEND	0x7B01
	#define IN_LOCK_DOCUMENT			0x7B02
	#define OUT_OK						0x7B03
	#define IN_CLOSE_CONNECTION			0x7B04
		

	//End of Reserved Commands
	
	/// <summary>
	/// 
	/// </summary>
	typedef struct tagBumbleDocument
	{
		CString *content;
		UINT nID;
		BOOL canRun;

	}BumbleDocument, * LPBumbleDocument;

	void NotifyDocument(UINT nWhat, BumbleDocument *pDoc);

	/// <summary>
	/// BumbleLangMode turns all supportive language modes into integral constants.
	/// </summary>
	enum class BumbleLangMode
	{
		Wollok = 0, Soy_Template, SH, PHP, Liquid, Hjson, ActionScript, RHTML, R, Maze, LaTeX, Jade, haXe,
		Handlebars, D, Curly, VHDL, Text, Smarty, SJS, RDoc, JSSM, HTML_Ruby, Forth, EJS, Dockerfile,
		Csound_Score, Clojure, XML, Typescript, snippets, Slim, pgSQL, Makefile, Kotlin, FSharp, Csound,
		C_SHARP, CoffeeScript, Pascal, Haskell_Cabal, Glsl, Elm, ColdFusion, AsciiDoc, VBScript, SCAD, Ruby,
		MATLAB, Io, HTML_Elixir, Gobstones, Gitignore, Gcode, Dot, Django, Terraform, Julia, INI, Verilog,
		SCSS, PHP_Blade_Template, Objective_C, MIXAL, LogiQL, Lisp, Visualforce, Stylus, SQL, Protobuf, Mask,
		Jack, GraphQLSchema, Gherkin, Edifact, Csound_Document, Apex, Assembly_x86, ABAP, YAML, Tcl, Rust,
		Puppet, Perl_6, Perl, Markdown, LESS, HAML, Groovy, Cirru, Cand_CPLUS_PLUS, TSX, Textile, RST, Lucene,
		LSL, JSX, JSP, FSL, Erlang, CSS, Cobol, ASL, ABC, Toml, Tex, Red, Python, MEL, LuaPage, JSONiq, Go,
		C9Search, Bro, AutoHotkey_AutoIt, XQuery, Swift, Razor, Properties, Prolog, Lua, JavaScript, Eiffel, 
		Diff, Apache_Conf, ADA, Twig, Praat, NSIS, FreeMarker, Vala, Space, Scala, Java, HTML, Velocity, SVG, 
		SQLServer, Scheme, SASS, Powershell, OCaml, MySQL, LiveScript, Elixir, Drools, Pig, Nix, MUSHCode, 
		JSON, Haskell, Fortran, Dart, BatchFile,
	};


	typedef UINT(CALLBACK* BUMBLETHREADPROC)(LPVOID);

	
	
	/// <summary>
	/// This structure contains the required information to create an instance of the extension. The field, gidReference,
	///  is mandatory to recognize the extension identically. An instance of this structure is filled and passed
	///  through the callback function to the framework
	/// </summary>
	typedef struct tagBumbleStartUpInfo {
		/*BUMBLETHREADPROC   lpfnThreadProc [Required]:
		A callback function to invoke from the dedicated thread (you process the raw content inside this function)*/
		BUMBLETHREADPROC	lpfnThreadProc;

		/*LPCWSTR	lpszExtName  [Required]:
		A meaningful name which is displayed in the extensions menu*/
		LPCWSTR				lpszExtName;

		/*GUID	gidReference [Required]:
		A GUID that avoids the naming conflicts of the extensions from different developers*/
		GUID				gidReference;

		/*UINT nIDThreadStart [optional]:
		Assign your command ID for the process starting under the dedicated thread inside the lpfnThreadProc.*/
		UINT				nIDThreadStart;

		/*UINT	nIDThreadStop [optional]:
		The dedicated thread terminates as soon as the running process completes. However, if the process gets
		trapped inside an infinite loop, you need to terminate it prematurely. To terminate
		(not recommended) the running process prematurely,   you can use a separate menu item. Assign this menu
		item's command ID to nIDThreadStop*/
		UINT				nIDThreadStop;


		/*
		*/
		UINT				nMenuID;


	}BumbleStartUpInfo, *LPBumbleStartUpInfo;

	


	class CBumbleExtInterface
	{
	public:

		~CBumbleExtInterface() = default;

		// Framework implemented functions
		//-------------------------------------------------------------------------------------------------------------
		/// <summary>
		///
		/// </summary>


		//-------------------------------------------------------------------------------------------------------------
		//User functions
		//-------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// This function is invoked when the user click on one of menu
		/// item in extension menu  
		/// </summary>
		/// <param name="nID"></param>
		/// <param name="nDocID"></param>
		virtual void OnCommand(UINT nID, UINT nDocID) = 0;

		/// <summary>
		/// Invoked by the framework at the beginning of the extension	You must fill all the fields
		/// in the BumbleStartUpInfo structure provided by the framework.
		/// </summary>
		/// <param name="lpBumbleInstance"></param>
		virtual void StartUp(BumbleStartUpInfo* lpBumbleInstance) = 0;

	};





}



#endif // HLEDTPLUGINBASE_H