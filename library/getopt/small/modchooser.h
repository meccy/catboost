#pragma once

#include <util/generic/map.h>
#include <util/generic/string.h>
#include <util/generic/vector.h>

#include <functional>

//! Mode function with vector of cli arguments.
typedef std::function<int(const yvector<TString>&)> TMainFunctionPtrV;
typedef int (*TMainFunctionRawPtrV)(const yvector<TString> &argv);

//! Mode function with classic argc and argv arguments.
typedef std::function<int(const int, const char **)> TMainFunctionPtr;
typedef int (*TMainFunctionRawPtr)(const int argc, const char **argv);

//! Mode class with vector of cli arguments.
class TMainClassV {
public:
    virtual int operator()(const yvector<TString> &argv) = 0;
    virtual ~TMainClassV() = default;
};

//! Mode class with classic argc and argv arguments.
class TMainClass {
public:
    virtual int operator()(const int argc, const char **argv) = 0;
    virtual ~TMainClass() = default;
};

//! Function to handle '--version' parameter
typedef void (*TVersionHandlerPtr)();

/*! Main class for handling different modes in single tool.
 *
 * You can add modes for this class, use autogenerated help with
 * list of modes and automaticly call necessary mode in run().
 *
 * In first argv element mode get joined by space tool name and
 * current mode name.
 */
class TModChooser {
public:
    TModChooser();
    ~TModChooser();
public:
    void AddMode(const TString& mode, const TMainFunctionRawPtr func, const TString& description);
    void AddMode(const TString& mode, const TMainFunctionRawPtrV func, const TString& description);
    void AddMode(const TString& mode, const TMainFunctionPtr func, const TString& description);
    void AddMode(const TString& mode, const TMainFunctionPtrV func, const TString& description);
    void AddMode(const TString& mode, TMainClass *func, const TString& description);
    void AddMode(const TString& mode, TMainClassV *func, const TString& description);

    void AddGroupModeDescription(const TString& description);

    //! Set main program description.
    void SetDescription(const TString& descr);

    //! Set modes help option name (-h is by default)
    void SetModesHelpOption(const TString& helpOption);

    //! Specify handler for '--version' parameter
    void SetVersionHandler(TVersionHandlerPtr handler);

    //! Set description show mode
    void SetSeparatedMode(bool separated = true);

    //! Set separation string
    void SetSeparationString(const TString& str);

    void DisableSvnRevisionOption();

    /*! Run appropriate mode.
     *
     * In this method following things happen:
     *   1) If first argument is -h/--help/-? then print short description of
     *      all modes and exit with zero code.
     *   2) If first argument is -v/--version and version handler is specified,
     *      then call it and exit with zero code.
     *   3) Find mode with the same name as first argument. If it's found then
     *      call it and return its return code.
     *   4) If appropriate mode is not found - return non-zero code.
     */
    int Run(const int argc, const char** argv) const;

    //! Run appropriate mode. Same as Run(const int, const char**)
    int Run(const yvector<TString> &argv) const;

    void PrintHelp(const TString& progName) const;

private:
    struct TMode {
        TString Name;
        TMainClassV *Main;
        TString Description;
        bool Separator;

        TMode()
            : Main(nullptr)
            , Separator(false)
        {
        }

        TMode(const TString& name, TMainClassV* main, const TString& descr);
    };

    typedef ymap<TString, TMode> TModes;

private:
    //! Main program description.
    TString Description;

    //! Help option for modes.
    TString ModesHelpOption;

    //! Wrappers around all modes.
    yvector<TAutoPtr<TMainClassV>> Wrappers;

    //! Modes
    TModes Modes;

    //! Handler for '--version' parameter
    TVersionHandlerPtr VersionHandler;

    //! When set to true, show descriptions unsorted and display separators
    bool ShowSeparated;

    //! When set to true, disables --svnrevision option, useful for opensource (git hosted) projects
    bool SvnRevisionOptionDisabled;

    //! Text string used when displaying each separator
    TString SeparationString;

    //! Unsorted list of options
    yvector<TMode> UnsortedModes;
};