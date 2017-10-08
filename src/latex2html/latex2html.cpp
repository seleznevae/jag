#include "latex2html/latex2html.h"
#include <assert.h>
#include <QDebug>

bool findLatexSubstringAndReplace(QString &str);

std::tuple<QString, bool> latex2html(const QString &str)
{
    QString ret(str);
    bool replacementsWere = findLatexSubstringAndReplace(ret);
    return std::tuple<QString, bool>(ret, replacementsWere);
}


/*!
  Returns position of latex equation substring and this substring.

  Latex equation substring is determined as follows "$something$". If such substring was not found than -1 as position
  and empty substring are returned. If latex equation substring is found it will be returned with $ signs.
*/
std::tuple<int, QString> findLatexEqSubstring(const QString &str)
{
    QChar prev = '\0';
    int eqStartPos = -1;
    int eqEndPos = -1;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '$' && prev != '\\') {
            if (eqStartPos == -1) {
                eqStartPos = i;
            } else if (eqEndPos == -1) {
                eqEndPos = i;
                break;
            }
        }
        prev = str[i];
    }

    if (eqStartPos >= 0 && eqEndPos >= 0) {
        return std::tuple<int, QString>(eqStartPos, str.mid(eqStartPos, eqEndPos - eqStartPos + 1));
    }
    return std::tuple<int, QString>(-1, "");
}


QString findAndReplaceGroup(const QString& str, QChar groupSymbol, const QString &addBefore, const QString &addAfter)
{
    QString ret(str);
    QChar prev = '\0';
    for (int i = 0; i < ret.size(); ++i) {
        if (prev == groupSymbol) {
            if (ret[i] != '{') {
                QString newString = addBefore + ret[i] + addAfter;
                ret.replace(i-1, 2, newString);
                i--;
            } else {
                QString newString = findAndReplaceGroup(ret.mid(i+1), groupSymbol, addBefore, addAfter);
                ret.replace(i+1, ret.size(), newString);
                auto it = std::find(ret.begin() + i, ret.end(), '}');
                if (it != ret.end()) {
                    int index = it - ret.begin();
                    QString newString = addBefore + ret.mid(i + 1, index - i - 1) + addAfter;
                    ret.replace(i-1, index - i + 2, newString);
                    i--;
                }
            }
        }
        prev = ret[i];
    }
    return ret;
}

void replaceLatexStringBySymbol(QString& str, const QString& pattern, QChar ch)
{
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\\') {

            if ((str.indexOf(pattern, i) == (i + 1))
                    && (str.size() == i + pattern.size() + 1 || !str[i + pattern.size() + 1].isLetterOrNumber())) {
                str.replace(i, pattern.size() + 1, QString(ch));
            }
        }
    }
}

QString replaceLatexByHtml(const QString& str)
{
    QString ret(str);
//    ret.replace("\\alpha", "&alpha;");
//    ret.replace("\\beta", "&beta;");
//    ret.replace("\\gamma", "&gamma;");
//    ret.replace("\\zeta", "&zeta;");
//    ret.replace("\\delta", "&delta;");


//    ret.replace("\\Alpha", "&Alpha;");
//    ret.replace("\\Beta", "&Beta;");
//    ret.replace("\\Gamma", "&Gamma;");
//    ret.replace("\\Delta", "&Delta;");
//    ret.replace("\\Epsilon", "&Epsilon;");
//    ret.replace("\\Zeta", "&Zeta;");
//    ret.replace("\\Eta", "&Eta;");
//    ret.replace("\\Theta", "&Theta;");
//    ret.replace("\\Iota", "&Iota;");
//    ret.replace("\\Kappa", "&Kappa;");
//    ret.replace("\\Lambda", "&Lambda;");

//    ret.replace("\\Mu", "&Mu;");
//    ret.replace("\\Nu", "&Nu;");
//    ret.replace("\\Xi", "&Xi;");
//    ret.replace("\\Omicron", "&Omicron;");
//    ret.replace("\\Pi", "&Pi;");
//    ret.replace("\\Rho", "&Rho;");


    replaceLatexStringBySymbol(ret, "Alpha"  , QChar(0x0391));
    replaceLatexStringBySymbol(ret, "Beta"   , QChar(0x0392));
    replaceLatexStringBySymbol(ret, "Gamma"  , QChar(0x0393));
    replaceLatexStringBySymbol(ret, "Delta"  , QChar(0x0394));
    replaceLatexStringBySymbol(ret, "Epsilon", QChar(0x0395));
    replaceLatexStringBySymbol(ret, "Zeta"   , QChar(0x0396));
    replaceLatexStringBySymbol(ret, "Eta"    , QChar(0x0397));
    replaceLatexStringBySymbol(ret, "Theta"  , QChar(0x0398));

    replaceLatexStringBySymbol(ret, "Iota"   , QChar(0x0399));
    replaceLatexStringBySymbol(ret, "Kappa"  , QChar(0x039A));
    replaceLatexStringBySymbol(ret, "Lambda" , QChar(0x039B));
    replaceLatexStringBySymbol(ret, "Mu"     , QChar(0x039C));
    replaceLatexStringBySymbol(ret, "Nu"     , QChar(0x039D));
    replaceLatexStringBySymbol(ret, "Xi"     , QChar(0x039E));
    replaceLatexStringBySymbol(ret, "Omicron", QChar(0x039F));
    replaceLatexStringBySymbol(ret, "Pi"     , QChar(0x03A0));

    replaceLatexStringBySymbol(ret, "Rho"    , QChar(0x03A1));
    replaceLatexStringBySymbol(ret, "Sigma"  , QChar(0x03A3));
    replaceLatexStringBySymbol(ret, "Tau"    , QChar(0x03A4));
    replaceLatexStringBySymbol(ret, "Upsilon", QChar(0x03A5));
    replaceLatexStringBySymbol(ret, "Phi"    , QChar(0x03A6));
    replaceLatexStringBySymbol(ret, "Chi"    , QChar(0x03A7));
    replaceLatexStringBySymbol(ret, "Psi"    , QChar(0x03A8));
    replaceLatexStringBySymbol(ret, "Omega"  , QChar(0x03A9));


    replaceLatexStringBySymbol(ret, "alpha"  , QChar(0x03B1));
    replaceLatexStringBySymbol(ret, "beta"   , QChar(0x03B2));
    replaceLatexStringBySymbol(ret, "gamma"  , QChar(0x03B3));
    replaceLatexStringBySymbol(ret, "delta"  , QChar(0x03B4));
    replaceLatexStringBySymbol(ret, "epsilon", QChar(0x03B5));
    replaceLatexStringBySymbol(ret, "zeta"   , QChar(0x03B6));
    replaceLatexStringBySymbol(ret, "eta"    , QChar(0x03B7));
    replaceLatexStringBySymbol(ret, "theta"  , QChar(0x03B8));

    replaceLatexStringBySymbol(ret, "iota"   , QChar(0x03B9));
    replaceLatexStringBySymbol(ret, "kappa"  , QChar(0x03BA));
    replaceLatexStringBySymbol(ret, "lambda" , QChar(0x03BB));
    replaceLatexStringBySymbol(ret, "mu"     , QChar(0x03BC));
    replaceLatexStringBySymbol(ret, "nu"     , QChar(0x03BD));
    replaceLatexStringBySymbol(ret, "xi"     , QChar(0x03BE));
    replaceLatexStringBySymbol(ret, "omicron", QChar(0x03BF));
    replaceLatexStringBySymbol(ret, "pi"     , QChar(0x03C0));

    replaceLatexStringBySymbol(ret, "rho"    , QChar(0x03C1));
    replaceLatexStringBySymbol(ret, "sigmaf" , QChar(0x03C2));
    replaceLatexStringBySymbol(ret, "sigma"  , QChar(0x03C3));
    replaceLatexStringBySymbol(ret, "tau"    , QChar(0x03C4));
    replaceLatexStringBySymbol(ret, "upsilon", QChar(0x03C5));
    replaceLatexStringBySymbol(ret, "phi"    , QChar(0x03C6));
    replaceLatexStringBySymbol(ret, "chi"    , QChar(0x03C7));
    replaceLatexStringBySymbol(ret, "psi"    , QChar(0x03C8));
    replaceLatexStringBySymbol(ret, "omega"  , QChar(0x03C9));

    replaceLatexStringBySymbol(ret, "thetasym", QChar(0x03D1));
    replaceLatexStringBySymbol(ret, "upsih"   , QChar(0x03D2));
    replaceLatexStringBySymbol(ret, "piv"     , QChar(0x03D6));

    replaceLatexStringBySymbol(ret, "weierp" , QChar(0x2118));
    replaceLatexStringBySymbol(ret, "image"  , QChar(0x2111));
    replaceLatexStringBySymbol(ret, "real"   , QChar(0x211C));
    replaceLatexStringBySymbol(ret, "trade"  , QChar(0x2122));
    replaceLatexStringBySymbol(ret, "alefsym", QChar(0x2135));

    ret = findAndReplaceGroup(ret, '^', "<sup>", "</sup>");
    ret = findAndReplaceGroup(ret, '_', "<sub>", "</sub>");
    return ret;
}



bool findAndReplaceEscapedCharacter(QString& str, QChar ch) {
    bool ret = false;
    QChar prev = '\0';
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ch && prev == '\\') {
            ret = true;
            i--;
            str.replace(i, 2, ch);
        }
        prev = str[i];
    }
    return ret;
}

bool findLatexSubstringAndReplace(QString &str)
{
    int pos;
    QString latexSubstring;
    std::tie(pos, latexSubstring) = findLatexEqSubstring(str);
    if  (pos != -1) {
        while (pos != -1) {
            //removing $ symbol
            int size = latexSubstring.size();
            latexSubstring.remove(0, 1);
            latexSubstring.remove(latexSubstring.size() - 1, 1);
            str.replace(pos, size, replaceLatexByHtml(latexSubstring));
            std::tie(pos, latexSubstring) = findLatexEqSubstring(str);
        }
        findAndReplaceEscapedCharacter(str, '$');
        return true;
    }
    return findAndReplaceEscapedCharacter(str, '$');
}




void testLatex2html()
{
    //findLatexEqSubstring testing
    {
        QString str = "asd";
        assert(std::get<0>(findLatexEqSubstring(str)) == -1);

        str = "asd $sdf ";
        assert(std::get<0>(findLatexEqSubstring(str)) == -1);

        str = "asd $sdf $ ";
        assert(std::get<0>(findLatexEqSubstring(str)) == 4);
        assert(std::get<1>(findLatexEqSubstring(str)) == "$sdf $");

        str = "asd \\$ $sdf $ ";
        assert(std::get<0>(findLatexEqSubstring(str)) == 7);
        assert(std::get<1>(findLatexEqSubstring(str)) == "$sdf $");

        str = "asd \\$ $sdf  ";
        assert(std::get<0>(findLatexEqSubstring(str)) == -1);

        str = "asd \\$ $sdf $ $123 $ ";
        assert(std::get<0>(findLatexEqSubstring(str)) == 7);
        assert(std::get<1>(findLatexEqSubstring(str)) == "$sdf $");
    }

    //findAndReplaceEscapedDollar testing
    {
        QString str = "asd";
        assert(findAndReplaceEscapedCharacter(str, '$') == false);

        str = "asd \\$ wqe";
        assert((findAndReplaceEscapedCharacter(str, '$') && str == "asd $ wqe"));

        str = "asd \\$ wqe \\$123 ";
        assert((findAndReplaceEscapedCharacter(str, '$') && str == "asd $ wqe $123 "));

    }

    //findLatexSubstringAndReplace testing
    {
        QString str = "asd";
        assert(findLatexSubstringAndReplace(str) == false);

        str = "asd $ wqe";
        assert(findLatexSubstringAndReplace(str) == false);

        str = "asd $ wqe \\$ qwe";
        assert(findLatexSubstringAndReplace(str) == true);
        assert(str == "asd $ wqe $ qwe");
    }

    //replaceLatexByHtml testing
    {
        QString str = "asd";
        assert(replaceLatexByHtml(str) == "asd");

        str = "asd \\$ wqe";
        assert(replaceLatexByHtml(str) == "asd \\$ wqe");

        str = "asd \\$ \\beta wqe";
        assert(replaceLatexByHtml(str) == QString("asd \\$ ") + QChar(0x03B2) + " wqe");
    }

    //latex2html testing
    {
        QString str = "asd";
        assert(std::get<1>(latex2html(str)) == false);

        str = "asd \\$ wqe";
        assert(std::get<1>(latex2html(str)) == true);
        assert(std::get<0>(latex2html(str)) == "asd $ wqe");

        str = "asd \\$ \\alpha wqe";
        assert(std::get<1>(latex2html(str)) == true);
        assert(std::get<0>(latex2html(str)) == "asd $ \\alpha wqe");

        str = "asd \\$ $a \\beta$ wqe";
        assert(std::get<1>(latex2html(str)) == true);
        assert(std::get<0>(latex2html(str)) == QString("asd $ a ") + QChar(0x03B2) + " wqe");
    }

    //findAndReplaceGroup testing
    {
        QString str = "asd asdf";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd asdf");

        str = "asd ^asdf";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd XXaYYsdf");

        str = "asd ^{a}sdf";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd XXaYYsdf");

        str = "asd ^{a r}sdf";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd XXa rYYsdf");

        str = "asd ^{a rsdf";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd ^{a rsdf");

        str = "asd ^{a} rs^{df dfg} 4";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd XXaYY rsXXdf dfgYY 4");

        str = "asd ^{a} rs^{df ^{dfg}} 4";
        assert(findAndReplaceGroup(str, '^', "XX", "YY") == "asd XXaYY rsXXdf XXdfgYYYY 4");
    }


    //replaceLatexStringBySymbol testing
    {
        QString str = "asd";
        assert((replaceLatexStringBySymbol(str, "\alpha", QChar('a')),  str == "asd"));

        str = "asd \\alpha sd";
        assert((replaceLatexStringBySymbol(str, "alpha", QChar('a')), qDebug() << str, str == "asd a sd"));

        str = "asd \\alpha s\\alphad d\\alpha;r";
        assert((replaceLatexStringBySymbol(str, "alpha", QChar('a')), qDebug() << str, str == "asd a s\\alphad da;r"));
    }

}

