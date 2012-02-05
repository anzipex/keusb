
\NeedsTeXFormat{LaTeX2e} % Нормоконтроллер Н.А. Сидорова
\ProvidesClass{mtkp}[2009/05/17 v1.1 MTKP Stencil] % ГОСТ 2.105-95

% Наследует класс:   extrarticle
% Подключает модули: inputenc mathtext caption geometry
%                    babel longtable indentfirst hyperref
%                    calc amsmath amsfonts fontenc array
%                    datetime fancyhdr perpage multirow
%                    graphicx makecell ifpdf

% Автор:  Муковников М.Ю. - mix_mix@pop3.ru - UIN 7817087
% Основа: Пакет eskdx - Кориков К.В. - http://lostclus.linux.kiev.ua/

% Changelog:
%
% 1.1  2009/05/17
%     > Добавлена четвертый уровень детализации 1.1.1.x
%     > Добавлены переносы формул по знаку операции
%     > Добавлены сокращенные команды для работ с таблицами и рисунками
%     ! Исправлен перенос строк в заголовках разделов по ГОСТ 2.105-95
%     ! Исправлено оформление заголовков приложений в содержании
%     ! Исправлено оформление списка литературы по ГОСТ 7.1-84
%
% 1.0  2009/05/04
%     * Реализация ГОСТ 2.105-95 для Московского Техникума
%       Космического Приборостроения
%

\newcommand{\MTKP@docfont}{14pt}
\newcommand{\MTKP@hack}{null}
\newcommand{\MTKP@float@style}{section}
\newcommand{\MTKP@equation@style}{section}

% Размер шрифта по-умолчанию
\DeclareOption{12pt}{\renewcommand{\MTKP@docfont}{12pt}}
\DeclareOption{14pt}{\renewcommand{\MTKP@docfont}{14pt}}

% Хак для корректной печати полей станицы моим <censored> Samsung ML-1520
\DeclareOption{hack}{\renewcommand{\MTKP@hack}{hack}}

% Тип нумерации таблиц и рисунков / формул
% (сквозная по всему документу или в границах раздела)
\DeclareOption{ftsingle}{\renewcommand{\MTKP@float@style}{single}}
\DeclareOption{ftsection}{\renewcommand{\MTKP@float@style}{section}}
\DeclareOption{eqsingle}{\renewcommand{\MTKP@equation@style}{single}}
\DeclareOption{eqsection}{\renewcommand{\MTKP@equation@style}{section}}

% Используемая кодировка документа
\DeclareOption{koi8-r}{\PassOptionsToPackage{\CurrentOption}{inputenc}}
\DeclareOption{cp1251}{\PassOptionsToPackage{\CurrentOption}{inputenc}}
\DeclareOption{cp866}{\PassOptionsToPackage{\CurrentOption}{inputenc}}
\DeclareOption{utf8}{\PassOptionsToPackage{\CurrentOption}{inputenc}}

\DeclareOption*{\edef\@tempa{\noexpand\setkeys{mtkp}{\CurrentOption}}\@tempa}
\ProcessOptions\relax

\LoadClass[a4paper,oneside,\MTKP@docfont]{extarticle}

\RequirePackage[warn]{mathtext}
\RequirePackage{inputenc}
\RequirePackage[english,russian]{babel}
\RequirePackage[T2A]{fontenc}
\RequirePackage{pscyr}
\RequirePackage{datetime}
\RequirePackage{float}

\linespread{1.25}
\renewcommand{\rmdefault}{ftm} % Times New Roman

\RequirePackage{calc}
\RequirePackage{geometry}
\RequirePackage{fancyhdr}
\RequirePackage{indentfirst}
\setlength{\parindent}{15mm}

% \newcommand{\MTKP@hack@null}{
%   \geometry{
%     tmargin=25mm+10mm,% Верхнее поле
%     inner=20mm+3mm,   % Левое поле
%     bottom=15mm+10mm, % Нижнее поле
%     outer=10mm+3mm}}  % Правое поле

\newcommand{\MTKP@hack@null}{
  \geometry{
    tmargin=25mm,% Верхнее поле
    inner=20mm,   % Левое поле
    bottom=15mm, % Нижнее поле
    outer=10mm}}  % Правое поле

\newcommand{\MTKP@hack@hack}{
  \geometry{
    tmargin=25mm+8.5mm, % Верхнее поле
    inner=20mm+0.5mm, % Левое поле
    bottom=15mm+13.5mm, % Нижнее поле
    outer=10mm+7mm}}  % Правое поле

\@nameuse{MTKP@hack@\MTKP@hack}

%
% Перечисления
%
\RequirePackage{enumitem} 

\renewcommand{\@listi}{\setlength{\itemsep}{0mm}}
\let\@listI\@listi
\@listi
\renewcommand{\alph}[1]{\asbuk{#1}}
 
\setenumerate[1]{label=\alph*), fullwidth, itemindent=\parindent, 
  listparindent=\parindent} 
\setenumerate[2]{label=\arabic*), fullwidth, itemindent=\parindent, 
  listparindent=\parindent, leftmargin=\parindent}
\setenumerate[3]{label=\bfseries{--}, fullwidth, itemindent=\parindent, 
  listparindent=\parindent, leftmargin=\parindent}
\setitemize[1]{label=\bfseries{--}, fullwidth, itemindent=\parindent, 
  listparindent=\parindent} 
\setitemize[2]{label=\bfseries{--}, fullwidth, itemindent=\parindent, 
  listparindent=\parindent, leftmargin=\parindent}
\setitemize[3]{label=\bfseries{--}, fullwidth, itemindent=\parindent, 
  listparindent=\parindent, leftmargin=\parindent}  

\newenvironment{enumerate*}{%
  \begingroup%
  \setenumerate[1]{label=\arabic*), fullwidth, itemindent=\parindent, 
    listparindent=\parindent} 
  \setenumerate[2]{label=\bfseries{--}, fullwidth, itemindent=\parindent, 
    listparindent=\parindent, leftmargin=\parindent}
  \begin{enumerate}{}{}}{%
  \end{enumerate}%
  \endgroup}

%
% Шрифты и надписи
%
\newcommand{\MTKPfontXsize}{\fontsize{9.7pt}{11pt}}    % 10pt
\newcommand{\MTKPfontXIIsize}{\fontsize{12pt}{13pt}}   % 12pt
\newcommand{\MTKPfontXIVsize}{\fontsize{14pt}{15pt}}   % 14pt
\newcommand{\MTKPfontXVIsize}{\fontsize{15.7pt}{17pt}} % 16pt
\newcommand{\MTKPfontXIXsize}{\fontsize{19pt}{21pt}}   % 19pt
\newcommand{\MTKPfontXXIIsize}{\fontsize{22pt}{24pt}}  % 22pt
\newcommand{\MTKPfontXXVIsize}{\fontsize{26pt}{28pt}}  % 26pt

\newcommand{\MTKPfontX}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXsize\selectfont\slshape}
\newcommand{\MTKPfontXII}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXIIsize\selectfont\slshape}
\newcommand{\MTKPfontXIV}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXIVsize\selectfont\slshape}
\newcommand{\MTKPfontXVI}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXVIsize\selectfont\slshape}
\newcommand{\MTKPfontXIX}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXIXsize\selectfont\slshape}
\newcommand{\MTKPfontXXII}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXXIIsize\selectfont\slshape}
\newcommand{\MTKPfontXXVI}{%
  \renewcommand{\baselinestretch}{1.2}
  \MTKPfontXXVIsize\selectfont\slshape}

%
% Разделы
%
\def\@startsection#1#2#3#4#5#6{%
  \if@noskipsec \leavevmode \fi
  \par
  \@tempskipa #4\relax
  \@afterindenttrue
  \ifdim \@tempskipa <\z@
    \@tempskipa -\@tempskipa \@afterindentfalse
  \fi
  \if@nobreak
    \everypar{}%
    \divide \@tempskipa by 2
    \vskip -\@tempskipa
    \vskip\z@skip
  \else
    \addpenalty\@secpenalty\addvspace\@tempskipa
  \fi
  \@ifstar
    {\@ssect{#3}{#4}{#5}{#6}}%
    {\@dblarg{\@sect{#1}{#2}{#3}{#4}{#5}{#6}}}}

\newcommand{\MTKPsectAlignLeft}{%
  \let\\\@centercr\@rightskip\@flushglue \rightskip\@rightskip%
  \leftskip\parindent}
\newcommand{\MTKPsectAlignRight}{%
  \let\\\@centercr\rightskip\z@skip\leftskip\@flushglue
  \parindent\z@\parfillskip\z@skip}
\newcommand{\MTKPsectAlignCenter}{%
  \let\\\@centercr
  \rightskip\@flushglue\leftskip\@flushglue
  \parindent\z@\parfillskip\z@skip}

\newcommand{\MTKPsectionAlign}{\MTKPsectAlignLeft}
\newcommand{\MTKPsubsectionAlign}{\MTKPsectAlignLeft}
\newcommand{\MTKPsubsubsectionAlign}{\MTKPsectAlignLeft}

\newcommand{\MTKPsectAlign}[2]{%
  \expandafter\renewcommand\csname\string MTKP#1Align\endcsname{%
  \@nameuse{MTKPsectAlign#2}}}

\newcommand{\MTKPsectionStyle}{\normalfont\MTKPfontXXVIsize\it\MakeUppercase}
\newcommand{\MTKPsubsectionStyle}{\normalfont\MTKPfontXXVIsize\it}
\newcommand{\MTKPsubsubsectionStyle}{\normalfont\MTKPfontXXIIsize\it}
\newcommand{\MTKPparagraphStyle}{\normalfont\MTKPfontXIXsize\it}

\renewcommand\section{\clearpage\@startsection {section}{1}{.em}%
                               {-15mm \@plus -3mm \@minus -2mm}%
                               {10mm \@plus 1mm \@minus 2mm}%
                               {\let\@hangfrom\noindent
                                \MTKPsectionAlign\MTKPsectionStyle}}

\renewcommand\subsection{\@startsection{subsection}{2}{.em}%
                               {-15mm \@plus -3mm \@minus -2mm}%
                               {10mm \@plus 1mm \@minus 2mm}%
                               {\let\@hangfrom\noindent
                                \MTKPsubsectionAlign\MTKPsubsectionStyle}}

\renewcommand\subsubsection{\@startsection{subsubsection}{3}{.em}%
                               {-10mm \@plus -3mm \@minus -2mm}%
                               {10mm \@plus 1mm \@minus 2mm}%
                               {\let\@hangfrom\noindent
                                \MTKPsubsubsectionAlign\MTKPsubsubsectionStyle}}

\renewcommand\paragraph{\@startsection{paragraph}{4}{.em}%
                               {-10mm \@plus -3mm \@minus -2mm}%
                               {10mm \@plus 1mm \@minus 2mm}%
                               {\let\@hangfrom\noindent
                                \MTKPsubsubsectionAlign\MTKPparagraphStyle}}

\renewcommand\subparagraph{\@startsection{subparagraph}{5}{.em}%
                               {-10mm \@plus -3mm \@minus -2mm}%
                               {10mm \@plus 1mm \@minus 2mm}%
                               {\let\@hangfrom\noindent
                                \MTKPsubsubsectionAlign\MTKPparagraphStyle}}

\newcommand\chapter{\@ifstar\chapternotoc\chapternormal}

\newcommand{\chapternotoc}[1]{
  \begingroup\clearpage\MTKPsectAlign{subsection}{Center}%
  \subsection*{#1}\endgroup}

\newcommand{\chapternormal}[1]{
  \begingroup\clearpage\MTKPsectAlign{subsection}{Center}%
  \subsection*{#1}%
  \addcontentsline{toc}{section}{#1}\endgroup}

\setcounter{secnumdepth}{4}
\renewcommand{\thesection}{\arabic{section}}
\renewcommand{\thesubsection}{\thesection.\arabic{subsection}}
\renewcommand{\thesubsubsection}{\thesubsection.\arabic{subsubsection}}
\renewcommand{\theparagraph}{\thesubsubsection.\arabic{paragraph}}

%
% Приложения
%
\newcounter{appendix}
\renewcommand{\theappendix}{\Asbuk{appendix}}

\renewcommand{\appendix}[2]{%
  \clearpage
  \refstepcounter{appendix}%
  \setcounter{section}{0}%
  \renewcommand{\thesection}{\theappendix.\arabic{section}}%
  \setcounter{equation}{0}%
  \renewcommand{\theequation}{\theappendix.\arabic{equation}}%
  \setcounter{figure}{0}%
  \renewcommand{\thefigure}{\theappendix.\arabic{figure}}%
  \setcounter{table}{0}%
  \renewcommand{\thetable}{\theappendix.\arabic{table}}%
  \addcontentsline{toc}{section}{\appendixname\ \theappendix.~#2 }%
  \begin{center}
    \MTKPsectionStyle\appendixname\ \theappendix%
    \ifx#1\@empty\else\\(#1)\fi\\#2%
  \end{center}
  \clearpage}

\def\@Asbuk#1{\ifcase#1\or
  \CYRA\or\CYRB\or\CYRV\or\CYRG\or\CYRD\or\CYRE\or\CYRZH\or
  \CYRI\or\CYRK\or\CYRL\or\CYRM\or\CYRN\or
  \CYRP\or\CYRR\or\CYRS\or\CYRT\or\CYRU\or\CYRF\or\CYRH\or
  \CYRC\or\CYRSH\or\CYRSHCH\or\CYREREV\or\CYRYU\or
  \CYRYA\else{\@tempcnta \number #1\relax
      \advance\@tempcnta -25\relax
      \protect\number\@tempcnta} % Алфавит для приложений
  \fi}
\def\@asbuk#1{\ifcase#1\or
  \cyra\or\cyrb\or\cyrv\or\cyrg\or\cyrd\or\cyre\or\cyrzh\or
  \cyri\or\cyrk\or\cyrl\or\cyrm\or\cyrn\or
  \cyrp\or\cyrr\or\cyrs\or\cyrt\or\cyru\or\cyrf\or\cyrh\or
  \cyrc\or\cyrsh\or\cyrshch\or\cyrerev\or\cyryu\or
  \cyrya\else{\@tempcnta \number #1\relax
      \advance\@tempcnta -25\relax
      \protect\number\@tempcnta}
  \fi}

%
% Содержание
%
\renewcommand*\l@section[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    \setlength\@tempdima{1.5em}%
    \begingroup
      \@tempdima 9.5mm \relax
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode 
      \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
      {#1\nobreak }%
      \leaders\hbox{%
        $\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep%
        mu$}\hfill%
      \nobreak%
      \hb@xt@\@pnumwidth{\hfil \normalcolor #2}%
      \par%
    \endgroup
  \fi}

\renewcommand*\l@subsection[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty%
    \setlength\@tempdima{1.5em}%
    \begingroup
      \@tempdima 15mm \relax
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode 
      \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
      {\hspace{10mm}{#1}\nobreak }%
      \leaders\hbox{%
        $\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep%
        mu$}\hfill%
      \nobreak%
      \hb@xt@\@pnumwidth{\hfil \normalcolor #2}%
      \par%
    \endgroup
  \fi}

\renewcommand*\l@subsubsection[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty%
    \setlength\@tempdima{1.5em}%
    \begingroup
      \@tempdima 17.5mm \relax
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode 
      \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
      {\hspace{22.5mm}{#1}\nobreak }%
      \leaders\hbox{%
        $\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep%
        mu$}\hfill%
      \nobreak%
      \hb@xt@\@pnumwidth{\hfil \normalcolor #2}%
      \par%
    \endgroup
  \fi}

\renewcommand*\l@paragraph[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty%
    \setlength\@tempdima{1.5em}%
    \begingroup
      \@tempdima 17.5mm \relax
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode 
      \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
      {\hspace{32.5mm}{#1}\nobreak }%
      \leaders\hbox{%
        $\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep%
        mu$}\hfill%
      \nobreak%
      \hb@xt@\@pnumwidth{\hfil \normalcolor #2}%
      \par%
    \endgroup
  \fi}

\RequirePackage[unicode]{hyperref}

\renewcommand\tableofcontents{%
  \begingroup
  \clearpage
  \MTKPsectAlign{subsection}{Center}
  \subsection*{\contentsname}
  \endgroup
  \@starttoc{toc}}

%
% Список используемой литературы
%
\renewcommand{\@biblabel}[1]{#1~}

\bibliographystyle{gost71s}

\def\thebibliography#1{%
      \chapter{\refname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
\def\endthebibliography{\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}

%
% Подписи к таблицам и рисункам
%
\RequirePackage{longtable}
\RequirePackage{caption}
\RequirePackage{multirow}
\RequirePackage{makecell}
\RequirePackage{array}
\DeclareCaptionLabelSeparator{dash}{ -- }

\newcommand\tablepersent[1]{#1\textwidth-2\tabcolsep-1.5\arrayrulewidth}
\newcommand\tablecolumn[2]{\multicolumn{1}{|m{\tablepersent{#1}}}{\centering#2}}
\newcommand\tablelastcolumn[2]{\multicolumn{1}{|m{\tablepersent{#1}}|}{\centering#2}}

\newcommand{\imghere}[4][]{%
  \begin{figure}[H]
    \begin{center}
      \includegraphics[#1]{#2}
    \end{center}
    \caption{#4}
    \label{#3}
  \end{figure}}

\setlength\LTleft{0pt}
\setlength\LTright\fill

\captionsetup[table]{%
labelsep=dash,justification=raggedright,singlelinecheck=false,%
aboveskip=3mm,belowskip=0mm}
\captionsetup[figure]{%
labelsep=dash,justification=centering,singlelinecheck=false,%
aboveskip=0mm,belowskip=3mm,font=it}

\addto\captionsrussian{%
  \def\tablename{{\cyr\CYRT\cyra\cyrb\cyrl\cyri\cyrc\cyra}}%
  \def\figurename{{\cyr\CYRR\cyri\cyrs\cyru\cyrn\cyro\cyrk}}%
  \def\refname{{\cyr\CYRS\cyrp\cyri\cyrs\cyro\cyrk\
    \cyri\cyrs\cyrp\cyro\cyrl\cyrsftsn\cyrz\cyru\cyre\cyrm\cyro\cyrishrt\
    \cyrl\cyri\cyrt\cyre\cyrr\cyra\cyrt\cyru\cyrr\cyrery}}%
  \def\mtkpname{{\CYRM\CYRO\CYRS\CYRK\CYRO\CYRV\CYRS\CYRK\CYRI\CYRISHRT\
    \CYRT\CYRE\CYRH\CYRN\CYRI\CYRK\CYRU\CYRM\
    \CYRK\CYRO\CYRS\CYRM\CYRI\CYRCH\CYRE\CYRS\CYRK\CYRO\CYRG\CYRO\
    \CYRP\CYRR\CYRI\CYRB\CYRO\CYRR\CYRO\CYRS\CYRT\CYRR\CYRO\CYRE\CYRN\CYRI\CYRYA}}
}

%
% Листинг программы
%
\RequirePackage{listings}
\lstset{showstringspaces=false,%
        tabsize=5,language=[GNU]C++,basicstyle=\MTKPfontX}

\newcommand\lst{\lstinline[basicstyle=\normalfont]}
\def\verbatim@font{\small\ttfamily\hyphenchar\font\m@ne\@noligs}

%
% Схема алгоритма
%
\RequirePackage{ifpdf}
\RequirePackage{graphicx}
\ifpdf\else\renewcommand{\includegraphics}[1][]{
  You'd use PDFLaTeX to build this document\\
  otherwise you'll see neither pictures nor flowcharts :(
  \\\url}\fi

%
% Нумерация плавающих объектов
%
\newcommand{\MTKP@float@style@single}{}

\newcommand{\MTKP@float@style@section}{
  \@addtoreset{figure}{section}
  \@addtoreset{table}{section}
  \renewcommand{\thefigure}{%
    \ifnum \c@section>\z@ \thesection.\fi \arabic{figure}}
  \renewcommand{\thetable}{%
    \ifnum \c@section>\z@ \thesection.\fi \arabic{table}}}

\@nameuse{MTKP@float@style@\MTKP@float@style}

\newcommand{\MTKP@equation@style@single}{}

\newcommand{\MTKP@equation@style@section}{
  \@addtoreset{equation}{section}
  \renewcommand{\theequation}{%
    \ifnum \c@section>\z@ \thesection.\fi \arabic{equation}}}

\@nameuse{MTKP@equation@style@\MTKP@equation@style}

%
% Сноски
%
\RequirePackage{perpage}
\MakePerPage[1]{footnote}

\let\footnotesize\MTKPfontXII
\def\footnote{\@ifnextchar[{\@xfootnote}{\stepcounter%
     {\@mpfn}\xdef\@thefnmark{\thempfn)}\@footnotemark\@footnotetext}}

\renewcommand{\footnoterule}{\vfill
     \hrule width .33\columnwidth
     \vspace*{2.6pt}}

\renewcommand\@makefntext[1]{%
  \parindent 15mm%
  \noindent
  \hb@xt@15mm{\hss\@makefnmark}~~#1}

%
% Переносы в формулах
%
\begingroup
\catcode`\+\active\gdef+{\mathchar8235\nobreak\discretionary{}%
  {\usefont{OT1}{cmr}{m}{n}\char43}{}}
\catcode`\*\active\gdef*{\mathchar8706\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char2}{}}
\catcode`\-\active\gdef-{\mathchar8704\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char0}{}}
\catcode`\=\active\gdef={\mathchar12349\nobreak\discretionary{}%
  {\usefont{OT1}{cmr}{m}{n}\char61}{}}
\catcode`\<\active\gdef<{\mathchar"313C\nobreak\discretionary{}%
  {\usefont{OML}{cmm}{m}{n}\char60}{}}
\catcode`\>\active\gdef>{\mathchar"313E\nobreak\discretionary{}%
  {\usefont{OML}{cmm}{m}{n}\char62}{}}
\endgroup
\def\times{\mathchar8706\nobreak\discretionary{}{\usefont{OMS}{cmsy}{m}{n}\char2}{}}
\def\subset{\mathchar"321A\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char26}{}}
\def\neq{\not=\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char54\usefont{OT1}{cmr}{m}{n}\char61}{}}
\def\sim{\mathchar"3218\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char24}{}}
\def\in{\mathchar"3232\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char50}{}}
\def\to{\mathchar"3221\nobreak\discretionary{}%
  {\usefont{OMS}{cmsy}{m}{n}\char33}{}} 

\RequirePackage{amsmath,amsfonts}

\AtBeginDocument{%
  \begingroup \catcode`\"=12
  \gdef\newmcodes@{\mathcode`\'39\mathcode`\*42\mathcode`\."613A%
  \mathcode`\-"8000\mathcode`\/47\mathcode`\:"603A\relax}%
  \endgroup
  \mathcode`\=="8000 \mathcode`\+="8000 %\mathcode`\-="8000
  \mathcode`\<="8000 \mathcode`\>="8000 \mathcode`\*="8000}

%
% Пояснения
%
\newenvironment{explanation}{%
  \begin{list}{}{\setlength{\parsep}{0pt}%
  \setlength{\topsep}{0pt}%
  \setlength{\itemsep}{0pt}%
  \setlength{\labelsep}{0pt}%
  \setlength{\labelwidth}{1.8em}%
  \setlength{\leftmargin}{1.8em}}}{%
  \end{list}}

%
% Количество элементов в документе
%
\let\MTKP@old@caption\@caption
\let\MTKP@old@bibitem\bibitem

\newcounter{MTKP@figure}
\newcounter{MTKP@table}
\newcounter{MTKP@bibitem}

\long\def\@caption#1[#2]#3{%
  \stepcounter{MTKP@#1}%
  \MTKP@old@caption{#1}[#2]{#3}}

\long\def\bibitem{%
  \stepcounter{MTKP@bibitem}%
  \MTKP@old@bibitem}

\def\MTKP@total@save#1#2{%
  \immediate\write\@auxout{%
    \string\gdef\string\MTKP@total@saved@\string#1{\arabic{#2}}}}

\AtEndDocument{%
  \MTKP@total@save{figure}{MTKP@figure}
  \MTKP@total@save{table}{MTKP@table}
  \MTKP@total@save{bibitem}{MTKP@bibitem}
  \MTKP@total@save{appendix}{appendix}
  \clearpage
  \addtocounter{page}{-1}
  \MTKP@total@save{page}{page}
  \addtocounter{page}{1}}

\newcommand{\MTKPtotal}[1]{%
  \@ifundefined{MTKP@total@saved@#1}{?}{%
    \@nameuse{MTKP@total@saved@#1}}}

%
% Титульный лист
%
\def\MTKPtitle#1{\gdef\@MTKPtitle{#1}}
\def\MTKPcode#1{\gdef\@MTKPcode{#1}}
\def\MTKPdate#1#2{\gdef\@MTKPdate{
    \twodigit{\number#1}.\twodigit{\number#2}.\number\year}}

\MTKPtitle{~}
\MTKPcode{~}
\MTKPdate{\day}{\month}

\renewcommand\maketitle{
  \begin{titlepage}
    \begin{center}
      \MTKPfontXVI\it{
        \mtkpname\vskip15pt
        \hfill{УТВЕРЖДАЮ}\\
        \hfill{Заместитель~директора}\\
        \hfill{по~учебной~работе}\\
        \hfill{Давыдова~А.\,А.}\\
        \vspace{15pt}
        %\hfill\@MTKPdate\hspace{24mm}~
        \vfill
      }
      \MTKPfontXXVI\it\MakeUppercase{\@MTKPtitle}\vfill
      \MTKPfontXXVI\it{Пояснительная~записка}\vfill
      \MTKPfontXXVI\it\@MTKPcode\vfill
      \MTKPfontXXVI\it{МП41-06}\vfill
      \MTKPfontXXVI\it{Листов~\MTKPtotal{page}}\vfill
      
      \MTKPfontXVI\it{Старший~консультант\hfillСидорова~Н.\,А.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXVI\it{Руководитель~разработки\hfillМуковников~Ю.\,А.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXVI\it{Консультант~по~экономической~части\hfillКардаш~Г.\,Г.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXVI\it{Рецензент\hfillХрапова~Л.\,А.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXVI\it{Председатель~предметной~комиссии\hfillЖилкина~Н.\,А.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXVI\it{Разработал\hfillМуковников~М.\,Ю.}~\vfill
      \vspace{15pt}
      %\MTKPfontXVI\hfill\it\@MTKPdate\hspace{24mm}~\vfill

      \MTKPfontXXII\it\number\year
    \end{center}
  \end{titlepage}

  \addtocounter{page}{1}
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
}

%
% Номера страниц
%
\geometry{headsep=10mm}
\pagestyle{fancy}
\fancyhf{}
\fancyhead[C]{%
  \MTKPfontXII--~\thepage~--\\\@MTKPcode%
} % установка верхнего колонтитула
\renewcommand{\headrulewidth}{0pt}

%
% 2009/05/17 v1.1 MTKP Stencil
%
