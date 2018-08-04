#include "Visit.H"

template void visitRPG<VisitorBase&>(VisitorBase& v);
template void visitRPG<const VisitorBase&>(const VisitorBase& v);
