/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[40];
    char stringdata0[11];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[18];
    char stringdata5[16];
    char stringdata6[6];
    char stringdata7[19];
    char stringdata8[16];
    char stringdata9[6];
    char stringdata10[16];
    char stringdata11[5];
    char stringdata12[14];
    char stringdata13[15];
    char stringdata14[13];
    char stringdata15[3];
    char stringdata16[17];
    char stringdata17[6];
    char stringdata18[22];
    char stringdata19[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 13),  // "onSendClicked"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 15),  // "onConfigureGroq"
        QT_MOC_LITERAL(42, 17),  // "onConfigureOllama"
        QT_MOC_LITERAL(60, 15),  // "onTokenReceived"
        QT_MOC_LITERAL(76, 5),  // "token"
        QT_MOC_LITERAL(82, 18),  // "onResponseFinished"
        QT_MOC_LITERAL(101, 15),  // "onErrorOccurred"
        QT_MOC_LITERAL(117, 5),  // "error"
        QT_MOC_LITERAL(123, 15),  // "onThemeSelected"
        QT_MOC_LITERAL(139, 4),  // "name"
        QT_MOC_LITERAL(144, 13),  // "onCustomTheme"
        QT_MOC_LITERAL(158, 14),  // "onManageThemes"
        QT_MOC_LITERAL(173, 12),  // "onPageLoaded"
        QT_MOC_LITERAL(186, 2),  // "ok"
        QT_MOC_LITERAL(189, 16),  // "onBackendChanged"
        QT_MOC_LITERAL(206, 5),  // "index"
        QT_MOC_LITERAL(212, 21),  // "onOllamaModelsChanged"
        QT_MOC_LITERAL(234, 6)   // "models"
    },
    "MainWindow",
    "onSendClicked",
    "",
    "onConfigureGroq",
    "onConfigureOllama",
    "onTokenReceived",
    "token",
    "onResponseFinished",
    "onErrorOccurred",
    "error",
    "onThemeSelected",
    "name",
    "onCustomTheme",
    "onManageThemes",
    "onPageLoaded",
    "ok",
    "onBackendChanged",
    "index",
    "onOllamaModelsChanged",
    "models"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    0,   88,    2, 0x08,    3 /* Private */,
       5,    1,   89,    2, 0x08,    4 /* Private */,
       7,    0,   92,    2, 0x08,    6 /* Private */,
       8,    1,   93,    2, 0x08,    7 /* Private */,
      10,    1,   96,    2, 0x08,    9 /* Private */,
      12,    0,   99,    2, 0x08,   11 /* Private */,
      13,    0,  100,    2, 0x08,   12 /* Private */,
      14,    1,  101,    2, 0x08,   13 /* Private */,
      16,    1,  104,    2, 0x08,   15 /* Private */,
      18,    1,  107,    2, 0x08,   17 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::QStringList,   19,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onSendClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConfigureGroq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConfigureOllama'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTokenReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onResponseFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onErrorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onThemeSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onCustomTheme'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onManageThemes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPageLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onBackendChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onOllamaModelsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onSendClicked(); break;
        case 1: _t->onConfigureGroq(); break;
        case 2: _t->onConfigureOllama(); break;
        case 3: _t->onTokenReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onResponseFinished(); break;
        case 5: _t->onErrorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onThemeSelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onCustomTheme(); break;
        case 8: _t->onManageThemes(); break;
        case 9: _t->onPageLoaded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->onBackendChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->onOllamaModelsChanged((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
