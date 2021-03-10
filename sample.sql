
## TEST SQL STRING ########################################################

 ## NOTE: about the examples used in to test the functionality of the code: 
 ## These sample texts are part of no concrete project and are designed specially to test the functionality
 ## of this code. Any similarity with any other concrete code from any project in the whole world 
 ## is simply coincidental. 

#QUERY_BEGIN:
#IFDEF FIRST_QUERY_DONE
    #DEFINE @UPDATE_EU
#ENDIF

#DEFINE @DATABASE_TYPE Excel 12.0
#DEFINE @LL            LL          
#DEFINE @UL            UL          
#DEFINE @EU            EU              

#DEFINE @LL00            LL                 
#DEFINE @UL00            UL          
#DEFINE @EU00            EU              

#DEFINE @NAME   Plan-Name    

#DEFINE @SIG        Signal
#DEFINE     @SIGNAL xPort-Name       

## 

#IFDEF @DATABASE_TYPE
    #DEFINE @DATABASE_PATH D:\Projects.012\Baku             
    #DEFINE @DATABASE_NAME TestPointList v01291.xls          
    #DEFINE @DST_TAB MSTVBRL                                
    #DEFINE @DST_RANGE $                
    #DEFINE @SRC_TAB Export                 
    #DEFINE @SRC_RANGE $                    
    
#ELSE
    #DEFINE @DATABASE_TYPE
    #DEFINE @DATABASE_PATH \\192.168.4.1\Projects.012\Baku                     
    #DEFINE @DATABASE_NAME      TPL_v01291.mdb
    #DEFINE @SRC_TAB MSTVBRL
    #DEFINE @SRC_RANGE 
    
#ENDIF

#DEFINE @QTEST____ ___defined_qtest___
#IFDEF @QTEST____
	#DEFINE @QTEST__20 ___defined_qtest__20_____
#ENDIF


UPDATE
	[@DATABASE_TYPE;IMEX=1;DATABASE=@DATABASE_PATH\@DATABASE_NAME].[@DST_TAB:@DST_RANGE] AS Dst,                
	[@DATABASE_TYPE;IMEX=1;DATABASE=@DATABASE_PATH\@DATABASE_NAME].[@SRC_TAB:@SRC_RANGE] AS Src                 
	    
SET
	Dst.[@LL] = Src.[@LL00], Dst.[@UL] = Src.[@UL00],
#IFDEF @UPDATE_EU                                  
	Dst.[@EU] = Src.[@EU00],
#ELSE
    -- EU will be entered manually (CC/21.02.12)
#ENDIF
    Dst.[Rem] = "upd"
#IFDEF @QTEST____
	 -- '@QTEST____' defined here as @QTEST____ (OK?)
	#IFDEF #DEFINE @QTEST__20
		-- '%         @QTEST__20%   ' __ defined here as @QTEST__20 .............
	#ENDIF
#ENDIF	
WHERE
	Left(Dst.[@NAME], InStr(Dst.[@NAME], "||")-1) = Src.[@NAME] AND -- remember to use "||"  
	Dst.[@SIG] = Src.[@SIGNAL]
	
;
#IFDEF FIRST_QUERY_DONE
    #GOTO QUERY_END
#ENDIF
#DEFINE FIRST_QUERY_DONE
#GOTO QUERY_BEGIN
#QUERY_END:

## EOF FOLLOWS



