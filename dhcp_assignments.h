#pragma once

#include <QVector>
#include <QString>

#include "static_assignment_t.h"

class dhcp_assignments_t
{
  public:
    dhcp_assignments_t();

    void AddAssignment( QString target, QString address );
    void RemoveAssignment( QString target );

    QVector<static_assignment_t> GetAssignments() const;
    const static_assignment_t& GetAssignment( int index ) const;

    int size() const { return m_assignments.size(); }
  private:
    QVector<static_assignment_t> m_assignments;
};
