#ifndef WALLETMODEL_H
#define WALLETMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "qjsonrpc/qjsonrpcsocket.h"
#include "qjsonrpc/qjsonrpcmessage.h"

class FundsTransaction
{
public:
    FundsTransaction()
    {}

    QString txId() const;
    void setTxId(const QString &txId);

    int outputs() const;
    void setOutputs(int outputs);

    int amountSatoshi() const;
    void setAmountSatoshi(int amountSatoshi);

private:
    QString m_txId;
    int m_outputs;
    int m_amountSatoshi;

};

class WalletModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int totalAvailableFunds READ totalAvailableFunds NOTIFY totalAvailableFundsChanged)
public:

    enum FundingRoles {
        TxidRole = Qt::UserRole + 1,
        OutputRole,
        SatoshiRole
    };

    WalletModel(QJsonRpcSocket* rpcSocket = 0);

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /*Q_INVOKABLE*/ int totalAvailableFunds();

signals:
    void totalAvailableFundsChanged();
    void newAddress(QString newAddress);

public slots:
    void requestNewAddress();

private slots:
    void listFundsRequestFinished();
    void newAddressRequestFinished();

private:
    QList<FundsTransaction> m_funds;
    QJsonRpcSocket* m_rpcSocket;

private:
    void fetchFunds();
    void populateFundsFromJson(QJsonArray jsonArray);
};

#endif // WALLETMODEL_H
