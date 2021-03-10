
 ## NOTE: about the examples used in to test the functionality of the code: 
 ## These sample texts are part of no concrete project and are designed specially to test the functionality
 ## of this code. Any similarity with any other concrete code from any project in the whole world 
 ## is just coincidental. 

## SAMPLE SQL CODE WITH MACROS
## DEFINITIONS OF MACROS:

#DEFINE @THIS_IS_A_TEST
##
#IFDEF      @THIS_IS_A_TEST
	#DEFINE     @TEST_DIR     ~~testFiles
#ENDIF

#DEFINE @GERMAN
###UNDEF  @GERMAN

#DEFINE @USE_CSV_FILES
#DEFINE @CSV_FILES_SUBDIR Csv_Files
#DEFINE @SERVER \\192.168.4.92

#IFDEF @SERVER
	#DEFINE @DATABASE_PATH @SERVER\PRO\Baku Unit1
#ELSE
	#DEFINE @DATABASE_PATH D:\PROJECTS.012\Baku\Baku Unit1
#ENDIF

#IFDEF @TEST_DIR
	#DEFINE @TMP__          @DATABASE_PATH          
	#UNDEF  @DATABASE_PATH                  
	#DEFINE @DATABASE_PATH  @TMP__\@TEST_DIR        
	#UNDEF  @TMP__

#ENDIF

#DEFINE @SRC_TAB        Sheet 1           
#DEFINE @DST_TAB        Sheet 2               
#DEFINE @SRC_RANGE      $A2:Q2567                             
#DEFINE @DST_RANGE      $A:AW                      

#DEFINE @NAME           Name              
#DEFINE @SIG            Sig
#DEFINE @SIGNAL         Signal

#DEFINE @LL             LL
#DEFINE @UL             UL 
#DEFINE @EU             EU
#DEFINE @LL00           Lower Limit
#DEFINE @UL00           Upper Limit
#DEFINE @EU00           Unit

###DEFINE @ENGLISH
###UNDEF @ENGLISH

#IFDEF @ENGLISH
    -- -- -- CONST '@ENGLISH' defined!!!!!
    #DEFINE @ENGLISH_DEFINED @TMP__!Dummy_English!
#ENDIF

#IFDEF      @ENGLISH_DEFINED
    #UNDEF @ENGLISH
    -- -- -- CONST '@ENGLISH_DEFINED' = "@ENGLISH_DEFINED" 
#ENDIF

#IFNDEF @ENGLISH
    -- -- -- CONST '@ENGLISH' not defined!!!!!
    #DEFINE @DUMMY Dummy Value
    #IFDEF @GERMAN
        #UNDEF @LL
        #UNDEF @UL
        #UNDEF @EU
        #UNDEF @LL00   
        #UNDEF @UL00
        #UNDEF @EU00
        ##
        #DEFINE @LL     UG
        #DEFINE @UL     OG
        #DEFINE @EU     EH
        #DEFINE @LL00   Untere Grenze
        #DEFINE @UL00   Obere Grenze
        #DEFINE @EU00   Einheit

    #ENDIF
    
#ENDIF

#IFDEF @DUMMY
-- Constant '@DUMMY' = @DUMMY
#ENDIF

#IFDEF @USE_CSV_FILES
	#DEFINE @DATABASE_NAME @CSV_FILES_SUBDIR
	#DEFINE @SRC_TAB Mel.csv
	#DEFINE @DST_TAB Hmi.csv
	#DEFINE @DATABASE_TYPE text
	
#ELSE
    #UNDEF @SRC_TAB
    #UNDEF @DST_TAB
    #UNDEF @SRC_RANGE
    #UNDEF @DST_RANGE

	#DEFINE @SRC_TAB Mel
	#DEFINE @DST_TAB Hmi
	#DEFINE @DATABASE_TYPE
	#IFDEF @USE_XLS_FILE
		#DEFINE     @DATABASE_NAME MelVel.xls
		#UNDEF      @SRC_RANGE
		#DEFINE     @SRC_RANGE $A:AW
		#UNDEF      @DATABASE_TYPE 
		#DEFINE     @DATABASE_TYPE Excel 12.0
		
	#ELSE
		#IFDEF @USE_ACCESS_FILE
			#DEFINE @DATABASE_NAME MelVel.accdb
			
		#ELSE
            #UNDEF  @SRC_RANGE
            #DEFINE @SRC_RANGE $
            #UNDEF  @DATABASE_TYPE 
            #DEFINE @DATABASE_TYPE Excel 12.0
		    #IFDEF  @TEST_DIR
                #DEFINE @DATABASE_NAME  Test.xlsx
                
            #ELSE
                #DEFINE @DATABASE_NAME  $THIS_WORKBOOK
                #UNDEF  @SRC_TAB
                #DEFINE @SRC_TAB $WORKSHEETS_02
                #UNDEF  @DST_TAB
                #DEFINE @DST_TAB $WORKSHEETS_01
                
			#ENDIF
			
		#ENDIF
		
	#ENDIF
	
#ENDIF

## THE QUERY:

UPDATE
	[@DATABASE_TYPE;IMEX=1;DATABASE=@DATABASE_PATH\@DATABASE_NAME].[@DST_TAB:@DST_RANGE] AS Dst,
	[@DATABASE_TYPE;IMEX=1;DATABASE=@DATABASE_PATH\@DATABASE_NAME].[@SRC_TAB:@SRC_RANGE] AS Src
	
SET
	Dst.[@LL] = Src.[@LL00],
	Dst.[@UL] = Src.[@UL00],
	Dst.[@EU] = Src.[@EU00],

WHERE
	Left(Dst.[@NAME], InStr(Dst.[@NAME], "||")-1) = Src.[@NAME] AND
	Dst.[@SIG] = Src.[@SIGNAL]              

;

## EOF 

