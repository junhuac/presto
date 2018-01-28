#ifndef PEERSMODEL_H
#define PEERSMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "qjsonrpc/qjsonrpcsocket.h"
#include "qjsonrpc/qjsonrpcmessage.h"

class Peer
{
public:
    Peer()
    {

    }

    // Copied over from c-lightning
    enum PeerState {
        UNINITIALIZED,
        /* Negotiating channel opening: in opening daemon */
        OPENINGD,
        /* In channeld, still waiting for lockin. */
        CHANNELD_AWAITING_LOCKIN,
        /* Normal operating state. */
        CHANNELD_NORMAL,
        /* We are closing, pending HTLC resolution. */
        CHANNELD_SHUTTING_DOWN,
        /* Exchanging signatures on closing tx. */
        CLOSINGD_SIGEXCHANGE,
        /* Waiting for onchain event. */
        CLOSINGD_COMPLETE,
        /* We've seen the funding spent, we're waiting for onchaind. */
        FUNDING_SPEND_SEEN,
        /* Various onchain states. */
        ONCHAIND_CHEATED,
        ONCHAIND_THEIR_UNILATERAL,
        ONCHAIND_OUR_UNILATERAL,
        ONCHAIND_MUTUAL
    };

    QString channel() const;
    void setChannel(const QString &channel);

    bool connected() const;
    void setConnected(bool connected);

    int msatoshiToUs() const;
    void setMsatoshiToUs(int msatoshiToUs);

    int msatoshiTotal() const;
    void setMsatoshiTotal(int msatoshiTotal);

    QString netAddress() const;
    void setNetAddress(const QString &netAddress);

    QString id() const;
    void setId(const QString &id);

    PeerState state() const;
    void setState(const PeerState &PeerState);

private:
    QString m_channel;
    bool m_connected;
    int m_msatoshiToUs;
    int m_msatoshiTotal;
    QString m_netAddress;
    QString m_id;
    PeerState m_state;
};

class PeersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PeerRoles {
        ChannelRole = Qt::UserRole + 1,
        ConnectedRole,
        MSatoshiToUsRole,
        MSatoshiTotalRole,
        NetAddressRole,
        PeerIdRole,
        PeerStateRole
    };

    QHash<int, QByteArray> roleNames() const;


    PeersModel(QJsonRpcSocket* rpcSocket = 0);
    void populatePeersFromJson(QJsonObject jsonObject);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void connectToPeer(QString peerId, QString peerAddress);

private slots:
    void listPeersRequestFinished();
    void connectToPeerRequestFinished();

private:
    void fetchPeers();

private:
    QList<Peer> m_peers;
    QJsonRpcSocket* m_rpcSocket;
};


#endif // PEERSMODEL_H
