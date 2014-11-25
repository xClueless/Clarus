#include "EndpointButton.hpp"

EndpointButton::EndpointButton(ChatGroup* group, QWidget *parent) :
	QPushButton(parent), mGroup(group)
{
	setLayout(&mLayout);

	setText(group->groupName());
	connect(mGroup, SIGNAL(groupNameChanged()), this, SLOT(updateLabel()));

	mLayout.addWidget(&mLabel);
}

ChatGroup* EndpointButton::group()
{
	return mGroup;
}

void EndpointButton::updateLabel()
{
	setText(mGroup->groupName());
}

