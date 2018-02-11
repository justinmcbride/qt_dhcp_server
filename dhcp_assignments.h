#pragma once

#include <QVector>
#include <QMap>
#include <QString>

#include <memory>

#include "static_assignment.h"
#include "mac_address.h"

class dhcp_assignments_t
{
  public:
    static std::shared_ptr<dhcp_assignments_t> get();

    void SetBaseAddress( QHostAddress new_base );

    void AddAssignment( QString target, QString address );
    void RemoveAssignment( QString target );

    QVector<static_assignment_t> GetAllStaticAssignments() const;
    const static_assignment_t& GetStaticAssignment( int index ) const;
    QHostAddress GetAssignment( mac_address_t client_address );

    int size() const { return m_assignments.size(); }
  private:
    dhcp_assignments_t();

    QMap<mac_address_t, static_assignment_t> m_assignments;
    QVector<static_assignment_t> m_assignments_static;

    QHostAddress m_address_base;

    int m_next_inc{ 1 };
};
