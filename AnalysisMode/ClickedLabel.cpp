#include "ClickedLabel.h"

void ClickedLabel::mouseReleaseEvent(QMouseEvent */*evt*/)
{
    emit Clicked();
}
