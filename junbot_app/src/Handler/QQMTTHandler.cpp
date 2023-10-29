#include "QMQTTHandler.h"
#include "Common.h"

QMqttHandler *QMqttHandler::getInstance()
{
    static QMqttHandler self;
    return &self;
}

QMqttHandler::QMqttHandler(QObject* parent)
    : QObject(parent)
{
//    RobotNode node_defalt;
//    node_defalt.current_state_topic = "robot1/state";

    QJsonObject jobj;

    jobj["control"] = "test";

    QJsonDocument jSub = QJsonDocument(jobj);
}


QMqttHandler::~QMqttHandler()
{

}

bool QMqttHandler::loadMQTTSetting(QString path)
{
    return true;
}

int QMqttHandler::initBokerHost(QString path)
{
    broker_path = path;

    return 0;
}

void QMqttHandler::connectMQTT(QString brokerName, qint16 port)
{
    m_client = new QMqttClient(this);

    m_client->setHostname(brokerName);
    m_client->setPort(port);
    m_client->connectToHost();

    connect(m_client, &QMqttClient::connected, this, &QMqttHandler::onMQTT_Connected);
    connect(m_client, &QMqttClient::disconnected, this, &QMqttHandler::onMQTT_disconnected);
    connect(m_client, &QMqttClient::messageReceived, this, &QMqttHandler::onMQTT_Received);
}

void QMqttHandler::onMQTT_Connected()
{
    LOG_DBG << "Connected to MQTT Broker";
    MQTT_Subcrib(m_current_robot_node);
}

void QMqttHandler::onMQTT_disconnected()
{
    LOG_DBG << "Disconnected to MQTT Broker";
}

void QMqttHandler::onMQTT_Received(const QByteArray &message, const QMqttTopicName &topic)
{
    LOG_DBG << "Received message: " << message << " from topic: " << topic.name();

    QJsonObject msg = QJsonDocument::fromJson(message).object();

    setMqttMessage(msg);

    emit MQTT_Received(m_mqttMessage);
}

void QMqttHandler::mqtt_Publish(RobotNode node, QJsonObject message)
{
    QMqttTopicName topic(node.control_topic);

    QJsonObject jobj;

    jobj["control"] = "test";

    m_client->publish(topic, QJsonDocument(jobj).toJson());
}

void QMqttHandler::pub(QString nod, QString message)
{
    QString topic = "robot1/control";

    QJsonObject jobj;

    jobj[nod] = message;

    m_client->publish(topic, QJsonDocument(jobj).toJson());
}

void QMqttHandler::pubRun(QString Target1, QString Target2, QString Target3)
{
    QString topic = "robot1/control";

    QJsonObject jobj;

    jobj["target1"] = Target1;
    jobj["target2"] = Target2;
    jobj["target3"] = Target3;

    m_client->publish(topic, QJsonDocument(jobj).toJson());
}

void QMqttHandler::MQTT_Subcrib(RobotNode node)
{
    LOG_DBG << node.current_state_topic;
    QMqttTopicFilter filter(node.current_state_topic);

    m_client->subscribe(filter);
}

void QMqttHandler::setMqttMessage(QJsonObject &msg)
{

}

void QMqttHandler::setCurrentRobotNode(RobotNode node)
{
    
}
