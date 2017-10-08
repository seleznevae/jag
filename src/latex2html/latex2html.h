#ifndef LATEX2HTML_H
#define LATEX2HTML_H

#include <tuple>
#include <QString>

std::tuple<QString, bool> latex2html(const QString &str);
void testLatex2html();

#endif // LATEX2HTML_H

