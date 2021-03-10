
## SQL EXAMPLE WITH MACRO CONDITIONS AND MACRO INTERPRET CONSTANT


#DEFINE @GERMAN

#IFNDEF @ENGLISH
    #IFDEF @GERMAN
        #DEFINE @SELECT_LINE ProjektNr, DataDate, Deadline, TätigkeitNr, Beschreibung, FrühereBeginn, SpätereEnde, IsKritisch, Verantwortlicher, Kommentar
        #DEFINE @CONDITION SpätestesEnde > Deadline and IsKritisch -- auf Deutsch
    #ELSE
        #IFDEF @FRENCH
            #DEFINE @SELECT_LINE projet_no, date_début, date_limite, nombre, déscription, début_plutôt, fin_retardé, est_critique, responsable, commentaire
            #DEFINE @CONDITION fin_retardé > date_limite and est_critique -- en français
        #ELSE
            #IFDEF @ROMANIAN
                #DEFINE @SELECT_LINE prnr, dd, dl, no, descr, es, ef, cr, resp, com
                #DEFINE @CONDITION ef > dl and cr
            #ENDIF
        #ENDIF
    #ENDIF
#ENDIF

## COMMENT: begin of query

select
#IFDEF @SELECT_LINE
    @SELECT_LINE
#ELSE
    project_id, data_date, deadline, task_id, task_descr, early_start, late_finish, is_critical, responsible, comment
#ENDIF
from
#IFNDEF @ENGLISH
    (select * from multilang_prj_source where
    -- comment: all projects in foreign languages in the same table with different fields
    #IFDEF @GERMAN
        -- if you see this comment, your project is in German
        language like '%@GERMAN%' and 
    #ELSE
        #IFDEF @FRENCH
            -- si vous voyez cette commentaire, votre projet est en français
            language like '%@FRENCH%' and
        #ELSE
            -- if you see this comment, your project is not in German nor in French
            #IFDEF @ROMANIAN
                language like '%@ROMANIAN%' and
            #ENDIF
        #ENDIF
        -- if you see this comment, your project _is_not_in_ German
    #ENDIF
    true)
#ELSE
    LastProject -- let's suppose, English project is separate table 
#ENDIF
where
#IFDEF @CONDITION
    @CONDITION
#ELSE
    late_finish > deadline and is_critical -- someting default
#ENDIF
;


## COMMENT: ~~end of query
