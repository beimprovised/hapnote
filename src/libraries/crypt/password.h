#ifndef __PASSWORD_H__
#define __PASSWORD_H__


#include <wobjectdefs.h>
#include <QObject>


#include <QString>

// Терминология:
// Ключ шифрования - это набор байт, полученный путем преобразования пароля.
//                   Механизм преобразования можно посмотреть в методе
//                   setCryptKeyToMemory(QString password)
// Промежуточный хеш - это набор байт, полученный путем преобразования пароля,
//                     по тому же алгоритму что и ключ шифрования,
//                     только без завершающего преобразования в MD5


#define SAVED_PASSWORD_CHECKING_LINE "This string is used for checking middle hash"

class Password : public QObject {
    W_OBJECT(Password)

public:
    Password();
    virtual ~Password();

    bool retrievePassword();
    bool replacePassword();
    void resetPassword(void);

    void setCryptKeyToMemory(QString password);

private:

    bool enterExistsPassword(void);
    bool checkPasswordWithExists(QString password);
    void saveCheckPasswordKey(QString password);

    QByteArray calculateMiddleHash(QString password);

    void saveMiddleHashCheckData(QByteArray middleHash);
    QByteArray calculateMiddleHashCheckData(QByteArray middleHash);

    void smartSaveMiddleHash(QString password);

    bool checkMiddleHash(void);

    void setCryptKeyToMemoryFromMiddleHash(void);

};

#endif // __PASSWORD_H__