// File: tcltk_button.cpp
#include <tcl.h>
#include <tk.h>
#include <iostream>

int main(int argc, char* argv[]) {
    Tcl_Interp* interp = Tcl_CreateInterp();

    if (Tcl_Init(interp) != TCL_OK) {
        std::cerr << "Tcl_Init error: " << Tcl_GetStringResult(interp) << "\n";
        return 1;
    }

    if (Tk_Init(interp) != TCL_OK) {
        std::cerr << "Tk_Init error: " << Tcl_GetStringResult(interp) << "\n";
        return 1;
    }

    Tcl_Eval(interp, "button .b -text \"Hello from Tk!\" -command {puts Hello}; pack .b");
    Tk_MainLoop();

    Tcl_DeleteInterp(interp);
    return 0;
}
