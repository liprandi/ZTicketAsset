#ifndef ZEMOJI_H
#define ZEMOJI_H

#include <QList>
#include <QString>


extern std::vector<QStringList> g_emoji;

class ZEmoji
{
public:
    ZEmoji(const QString& emoji)
    {
        size_t index = 0;
        m_emoji = g_emoji[0];
        m_html = g_emoji[0][1];
        m_index = 0;
        for(const auto& item: g_emoji) {
            if(!item[0].compare(emoji)) {
                m_emoji = item;
                m_index = index;
                m_html = m_emoji[1];
                break;
            }
            index++;
        }
    }
    ZEmoji(size_t index)
    {
        m_emoji = g_emoji[0];
        m_html = g_emoji[0][1];
        m_index = 0;
        if(index >= 0 && index < g_emoji.size())
        {
            m_emoji = g_emoji[index];
            m_index = index;
            m_html = m_emoji[1];
        }
    }

    ZEmoji(const ZEmoji& emoji)
    {
        m_emoji = emoji.m_emoji;
        m_index = emoji.m_index;
        m_html = emoji.m_html;
    }

    static ZEmoji FromStatus(const QString& status)
    {

        size_t index = 0;
        ZEmoji emoji(0);
        for(const auto& item: g_emoji) {
            if(!item[1].compare(status)) {
                emoji = ZEmoji(index);
                break;
            }
            index++;
        }
        return emoji;
    }

public:
    QStringList m_emoji;
    size_t m_index;
    QString m_html;
};

#endif // ZEMOJI_H
