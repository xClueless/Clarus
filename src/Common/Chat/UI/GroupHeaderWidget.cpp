#include "GroupHeaderWidget.hpp"

GroupHeaderWidget::GroupHeaderWidget(ChatGroup* group, QWidget* parent)
	: QWidget(parent), mGroup(group)
{
	mLayout = new QFormLayout(this);
	setLayout(mLayout);

	createHeaderBox();
	mLayout->addWidget(mHeaderBox);

	mGroupEndpointsLabel = new QLabel(mGroup->endpointRemoteNames().join(','));

	connect(mGroup, SIGNAL(groupNameChanged()), this, SLOT(updateGroupNameLabel()));
	connect(mGroup, SIGNAL(endpointAdded(MessageEndpoint*)), this, SLOT(handleEndpointAddtion(MessageEndpoint*)));
	connect(mGroup, SIGNAL(endpointRemoved(MessageEndpoint*)), this, SLOT(handleEndpointRemoval(MessageEndpoint*)));
}

void GroupHeaderWidget::updateGroupNameLabel()
{
	mGroupNameLabel->setText(mGroup->groupName());
}

void GroupHeaderWidget::updateEndpointsLabel()
{
	mGroupEndpointsLabel->setText(mGroup->endpointRemoteNames().join(','));
}

void GroupHeaderWidget::createHeaderBox()
{
	mHeaderBox = new QGroupBox(this);
	QHBoxLayout* headerBoxLayout = new QHBoxLayout;

	mGroupIconLabel = new QLabel(mHeaderBox);
	mGroupIconLabel->setPixmap(mGroup->groupPixmap());
	headerBoxLayout->addWidget(mGroupIconLabel);

	mGroupNameLabel = new QLabel(mGroup->groupName(), mHeaderBox);
	headerBoxLayout->addWidget(mGroupNameLabel);

	mAudioCallButton = new QPushButton(mHeaderBox);
	mAudioCallIcon.addFile(AUDIO_CALL_ICON_PATH, QSize(48, 48));
	mAudioCallButton->setIcon(mAudioCallIcon);
	headerBoxLayout->addWidget(mAudioCallButton);

	mVideoCallButton = new QPushButton(mHeaderBox);
	mVideoCallIcon.addFile(VIDEO_CALL_ICON_PATH);
	mVideoCallButton->setIcon(mVideoCallIcon);
	headerBoxLayout->addWidget(mVideoCallButton);

	mAddEndpointButton = new QPushButton("+", mHeaderBox);
	connect(mAddEndpointButton, SIGNAL(clicked()), this, SLOT(addEndpoint()));
	headerBoxLayout->addWidget(mAddEndpointButton);

	mHeaderBox->setLayout(headerBoxLayout);
}


void GroupHeaderWidget::handleEndpointAddtion(MessageEndpoint* endpoint)
{
	updateEndpointsLabel();
}

void GroupHeaderWidget::handleEndpointRemoval(MessageEndpoint* endpoint)
{
	updateEndpointsLabel();
}

void GroupHeaderWidget::addEndpoint()
{

}
