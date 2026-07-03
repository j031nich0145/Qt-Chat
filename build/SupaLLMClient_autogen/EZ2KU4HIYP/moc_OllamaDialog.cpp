/****************************************************************************
** Meta object code from reading C++ file 'OllamaDialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ollama/OllamaDialog.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OllamaDialog.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_OllamaDialog_t {
    uint offsetsAndSizes[26];
    char stringdata0[13];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[16];
    char stringdata5[25];
    char stringdata6[7];
    char stringdata7[15];
    char stringdata8[5];
    char stringdata9[15];
    char stringdata10[4];
    char stringdata11[8];
    char stringdata12[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OllamaDialog_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OllamaDialog_t qt_meta_stringdata_OllamaDialog = {
    {
        QT_MOC_LITERAL(0, 12),  // "OllamaDialog"
        QT_MOC_LITERAL(13, 16),  // "onInstallClicked"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 13),  // "onPullClicked"
        QT_MOC_LITERAL(45, 15),  // "onRemoveClicked"
        QT_MOC_LITERAL(61, 24),  // "onInstalledModelsChanged"
        QT_MOC_LITERAL(86, 6),  // "models"
        QT_MOC_LITERAL(93, 14),  // "onPullProgress"
        QT_MOC_LITERAL(108, 4),  // "line"
        QT_MOC_LITERAL(113, 14),  // "onPullFinished"
        QT_MOC_LITERAL(128, 3),  // "tag"
        QT_MOC_LITERAL(132, 7),  // "success"
        QT_MOC_LITERAL(140, 16)   // "onRemoveFinished"
    },
    "OllamaDialog",
    "onInstallClicked",
    "",
    "onPullClicked",
    "onRemoveClicked",
    "onInstalledModelsChanged",
    "models",
    "onPullProgress",
    "line",
    "onPullFinished",
    "tag",
    "success",
    "onRemoveFinished"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OllamaDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    1,   59,    2, 0x08,    4 /* Private */,
       7,    1,   62,    2, 0x08,    6 /* Private */,
       9,    2,   65,    2, 0x08,    8 /* Private */,
      12,    2,   70,    2, 0x08,   11 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   10,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject OllamaDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_OllamaDialog.offsetsAndSizes,
    qt_meta_data_OllamaDialog,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OllamaDialog_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OllamaDialog, std::true_type>,
        // method 'onInstallClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPullClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRemoveClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onInstalledModelsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'onPullProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onPullFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onRemoveFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void OllamaDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OllamaDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onInstallClicked(); break;
        case 1: _t->onPullClicked(); break;
        case 2: _t->onRemoveClicked(); break;
        case 3: _t->onInstalledModelsChanged((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 4: _t->onPullProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->onPullFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->onRemoveFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *OllamaDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OllamaDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OllamaDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int OllamaDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
