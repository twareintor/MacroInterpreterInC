# MacroInterpreterInC
Introducing macros in code who doesn't support this feature.
<pre>
Motivation:
Let's take the example of a pure SQL query string, formatted as to be clearly understood and ready for further development.
Let's be it a SQL string for Access. Beside that Access doesn't let you even change the font face nor size, the SQL string is not saved with the original format. As were not enough, this doesn't support even comments. But this is another story that is subject of anther project of mine which, if not already, will be available on GitHug as soon as possible.
Let's get to the motivation for this project. Let's have a SQL string to update a table. This will look similar to:

UPDATE
    [<Database1>].[<Table1>] AS Dst
INNER JOIN
    [<Database2>].[<Table2>] AS Src
ON
    Dst.[<Field1>] = Src.[<Field2>]
SET
    Dst.[<TargetField>] = MID(Src.[<SourceField>], INSTR(Src.[<SourceField>], Dst.[<TargetField>]))
WHERE
    INSTR(Src.[<SourceField>], Dst.[<TargetField>])
;

Before let it run, you may want to check to have a look at the possible results. Let's suppose your database development environment doesn't offer you this possibility for free.
So, you need a similar query to check what-it-will-be after update which will look like this:

SELECT
    Dst.[<TargetField>], "==>" AS Op, MID(Src.[<SourceField>], INSTR(Src.[<SourceField>], Dst.[<TargetField>]))
FROM
    [<Database1>].[<Table1>] AS Dst
INNER JOIN
    [<Database2>].[<Table2>] AS Src
ON
    Dst.[<Field1>] = Src.[<Field2>]
WHERE
    INSTR(Src.[<SourceField>], Dst.[<TargetField>])
;
    
The two queries looks quite similar. But if you change sometihng in one, you have to change the same in another. Othwerwise, you can see, in check-phase, results which wil not be exactly what you'll get in the destination table. And this is not good.

Solution:
What if I have the same code for both activities? For check and update the same code. Note: the same code, that will output two different queries if we simply switch a macro definition. Something like this below:

#DEFINE @THIS_IS_THE_UPDATE

#IFNDEF @THIS_IS_THE_UPDATE
SELECT
    Dst.[<TargetField>], "==>" AS Op, MID(Src.[<SourceField>], INSTR(Src.[<SourceField>], Dst.[<TargetField>]))
FROM
#ELSE
UPDATE
    [<Database1>].[<Table1>] AS Dst
INNER JOIN
#ENDIF
    [<Database2>].[<Table2>] AS Src
ON
    Dst.[<Field1>] = Src.[<Field2>]
#IFDEF @THIS_IS_THE_UPDATE
SET
    Dst.[<TargetField>] = MID(Src.[<SourceField>], INSTR(Src.[<SourceField>], Dst.[<TargetField>]))
#ENDIF
WHERE
    INSTR(Src.[<SourceField>], Dst.[<TargetField>])
;
  
In this example, simply removing the @THIS_IS_THE_UPDATE definition will change the query, resulting in a select one or in an update one.

Conclusion:
This is just a simple example and a possible simple application of it. Is intended to support complete definitions like
#DEFINE @DATABASE Excel 12.0;imex=0;database=c:\path\to\filename.xls
#DEFINE @TABLE01 Sheet1$
To support embraced #IF../#ELSE/#ENDIF conditions, at as many levels as possible
An unrecognized macro command is ignored and becomes just a macro comment
Intended to support also something like #INCLUDE directive 
And more.
</pre>
