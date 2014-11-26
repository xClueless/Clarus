#ifndef GROUPHEADERWIDGET_HPP
#define GROUPHEADERWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QIcon>
#include "../ChatGroup.hpp"

class GroupHeaderWidget : public QWidget
{
private:
	Q_OBJECT
	ChatGroup* mGroup;
	QFormLayout* mLayout;

	QGroupBox* mHeaderBox;
	QLabel* mGroupIconLabel;

	QLabel* mGroupNameLabel;

	QPushButton* mAudioCallButton;
	QIcon mAudioCallIcon;
	const QString AUDIO_CALL_ICON_PATH = ":/images/audio-call.png";

	QPushButton* mVideoCallButton;
	QIcon mVideoCallIcon;
	const QString VIDEO_CALL_ICON_PATH = ":/images/video-call.png";

	QPushButton* mAddEndpointButton;

	QLabel* mGroupEndpointsLabel;

	void createHeaderBox();
public:
	explicit GroupHeaderWidget(ChatGroup* group, QWidget *parent = 0);

signals:

public slots:
	void updateGroupNameLabel();
	void updateEndpointsLabel();
	void handleEndpointAddtion(MessageEndpoint* endpoint);
	void handleEndpointRemoval(MessageEndpoint* endpoint);
	void addEndpoint();
};

#endif // GROUPHEADERWIDGET_HPP
