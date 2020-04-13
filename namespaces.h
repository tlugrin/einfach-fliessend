#ifndef NAMESPACES_H
#define NAMESPACES_H


namespace Langue {
    enum LangueApp {de=0,fr=1,en=2};
    enum LangueVoc {Fr_de=0,Fr_en=1,Fr_sh=2};// TODO: use to simplify code
}

Q_DECLARE_METATYPE(Langue::LangueApp)

#endif // NAMESPACES_H

