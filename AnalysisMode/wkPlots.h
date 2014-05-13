#ifndef WKPLOTS_H
#define WKPLOTS_H
class QString;
class wkPlots
{
public:
    virtual setGraphTitle(const QString&)=0;
    virtual setXScale(const double & xmin, const double & xmax)=0;
    virtual setXLabel(const QString &xLable)=0;

    virtual setYLeftScale(const double & xmin, const double & xmax)=0;
    virtual setYLeftLabel(const QString &xLable)=0;
    virtual setYRightScale(const double & xmin, const double & xmax)=0;
    virtual setYRightLabel(const QString &xLable)=0;

    virtual setXLog(const bool& value)=0;
    virtual setYLeftLog(const bool & value)=0;


    ~wkPlots();
};


#endif // WKPLOTS_H
