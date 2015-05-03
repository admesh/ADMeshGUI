packagesExist(brew) {
    BREW_PREFIX = $$system(brew --prefix)
    exists($$BREW_PREFIX/opt/admesh/stl.h){
        QMAKE_CXXFLAGS += -I/usr/local/opt/admesh/include
        LIBS += -L/usr/local/opt/admesh/lib
    }
    exists($$BREW_PREFIX/opt/stlsplit/*){
        QMAKE_CXXFLAGS += -I/usr/local/opt/stlsplit/include
        LIBS += -L/usr/local/opt/stlsplit/lib
    }
    exists($$BREW_PREFIX/opt/gettext/*){
        QMAKE_CXXFLAGS += -I/usr/local/opt/gettext/include
        LIBS += -L/usr/local/opt/gettext/lib
    }
}

