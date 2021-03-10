
























select

    ProjektNr, DataDate, Deadline, TätigkeitNr, Beschreibung, FrühereBeginn, SpätereEnde, IsKritisch, Verantwortlicher, Kommentar


from

    (select * from multilang_prj_source where
    -- comment: all projects in foreign languages in the same table with different fields

        -- if you see this comment, your project is in German
        language like '%@GERMAN%' and 







    true)


where

    SpätestesEnde > Deadline and IsKritisch -- auf Deutsch


;



