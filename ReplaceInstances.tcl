# Main function is phLEditReplaceAllInstOfCell
# It will attempt to replace all isntances of the cell given by old<lib/cell/view>
# If there is less than 2GB of RAM available the function will fall through without changing

proc phGetTextTypes {} {
	return {spice veriloga verilogams verilog vhdl vhdlams}
}

# Only for windows
proc phGetTotalRAM {} {
	if {$::tcl_platform(platform)=="windows"} {
		set mem [exec wmic OS get TotalVisibleMemorySize]
		return [lindex $mem 1]
	}
	return ""
}

# Only for windows
proc phGetAvailableRAM {} {
	if {$::tcl_platform(platform)=="windows"} {
		set mem [exec wmic OS get FreePhysicalMemory]
		return [lindex $mem 1]
	}
	return ""
}

proc phLEditReplaceAllInstOfCell {oldlib oldcell oldview newlib newcell newview} {
	set memory_limit [expr 2*1024*1024]
	set inst_list [phLEditFindInstances $oldlib $oldcell $oldview]
	set pCell [LCell_GetVisible]
	puts "[llength $inst_list] instances found"
	set count 0
	set completion 0
	set completion_old 0
	foreach {inst} $inst_list {
		if {[phLEditIsValidCellHandle $inst]} {
			set freeMem [phGetAvailableRAM]
			if {$freeMem < $memory_limit} {
				puts "Error: Memory limit ($memory_limit) reached, stopping."
				break
			} else {
				phLEditReplaceInstWithNewCell $pCell $inst $newlib $newcell $newview
			}
		}
		incr count
		if {[llength $inst_list] > 10} {
			set completion [expr 100*$count/[llength $inst_list]]
			if {$completion > $completion_old} {
				puts "$completion\% replaced."
				set completion_old [expr $completion + 5]
				update
			}
		}
	}
	LDisplay_Refresh
	LCell_ClearUndoLists $pCell
	unset inst_list
}

proc phLEditReplaceInstWithNewCell {pCell inst newlib newcell newview} {
	set transform [LInstance_GetTransform_Ex99 $inst]
	set new_cell [LCell_FindEx2 $newcell $newview $newlib]
	if {[phLEditIsValidCellHandle $new_cell]} {
		set name [LInstance_GetName $inst]
		LInstance_Delete $pCell $inst
		set inst_new [LInstance_New_Ex99 $pCell $new_cell $transform {1 1} {0 0}]
		LInstance_SetName $pCell $inst_new $name
		unset inst_new
		unset inst
	} else {
		puts "Unable to find $newlib/$newcell/$newview"
	}
}

proc phLEditIsValidCellHandle {pcell} {
	if {[llength $pcell]>0 && ![string equal $pcell "{}"]} {
		return 1
	} else {
		return 0
	}
}

proc phLEditFindInstances {library cellname viewname} {
	set pCell [LCell_GetVisible]
	set objs [LInstance_GetList $pCell]
	set instances {}
	foreach {inst} $objs {
		set mCell [LInstance_GetCell $inst]
		set gCell [LCell_GetGenerator $mCell]
		if {[llength $gCell]>0 && ![string equal $gCell "{}"]} {
			set mCell $gCell
		}
		set lib [LCell_GetLibName $mCell]
		set view [LCell_GetViewName $mCell]
		set cell [LCell_GetName $mCell]
		if {[string equal $lib $library] && [string equal $cellname $cell] && [string equal $viewname $view]} {
			lappend instances $inst
		}
	}
	unset objs
	return $instances
}

proc main {} {
  set oldlib "pragLib"
  set oldcell "res"
  set oldview "layout"
  set newlib "pragLib"
  set newcell "nch"
  set newview $oldview
  phLEditReplaceAllInstOfCell $oldlib $oldcell $oldview $newlib $newcell $newview
}

