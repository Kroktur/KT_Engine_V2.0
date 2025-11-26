//#include "CloudPoint.h"
//
//Edge::Edge(const Point3F& p, const Point3F& p2) : A(p), B(p2)
//{
//}
//
// bool Edge::operator==(const Edge& other) const
//{
//	if ((A == other.A && B == other.B) || (A == other.B && B == other.A))
//		return true;
//	return false;
//}
//bool Edge::operator!=(const Edge& other) const
//{
//	return !(*this == other);
//}
//
//Vector3F Edge::ToVec() const 
//{
//	return Vector3F(B - A);
//}
//
//bool Edge::IsIntersect(const Edge& other) const
//{
//	if (!IsCoplanar(other))
//		return false;
//
//    Vector3F p = A;
//    Vector3F r = B - A;
//    Vector3F q = other.A;
//    Vector3F s = other.B - other.A;
//
//    Vector3F rxs = r.Cross(s);
//    Vector3F q_p = q - p;
//    float rxs_len2 = rxs.Dot(rxs);
//
//    if (rxs_len2 < Math::EPSILON_FLOAT) {
//        
//        Vector3F q_p_cross_r = q_p.Cross(r);
//        if (q_p_cross_r.Dot(q_p_cross_r) < Math::EPSILON_FLOAT) {
//          
//            float t0 = q_p.Dot(r) / r.Dot(r);
//            float t1 = (q + s - p).Dot(r) / r.Dot(r);
//            if (t0 > t1) std::swap(t0, t1);
//            return t0 <= 1.0f && t1 >= 0.0f;
//        }
//        return false; 
//    }
//
//   
//    float t = q_p.Cross(s).Dot(rxs) / rxs_len2;
//    float u = q_p.Cross(r).Dot(rxs) / rxs_len2;
//
//    return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
//}
//
//Vector3F Edge::InterSect(const Edge& other) const
//{
//    if (!IsIntersect(other))
//        throw std::runtime_error("Segments do not intersect");
//    Vector3F p = A;
//    Vector3F r = B - A;
//    Vector3F q = other.A;
//    Vector3F s = other.B - other.A;
//
//    Vector3F rxs = r.Cross(s);
//    Vector3F q_p = q - p;
//    float rxs_len2 = rxs.Dot(rxs);
//
//    if (rxs_len2 < Math::EPSILON_FLOAT) {
//
//        Vector3F q_p_cross_r = q_p.Cross(r);
//        if (q_p_cross_r.Dot(q_p_cross_r) < Math::EPSILON_FLOAT) {
//
//            float t0 = q_p.Dot(r) / r.Dot(r);
//            float t1 = (q + s - p).Dot(r) / r.Dot(r);
//            if (t0 > t1) std::swap(t0, t1);
//
//            return p +  r * std::max(0.0f, t0);
//        }
//    }
//
//
//    float t = q_p.Cross(s).Dot(rxs) / rxs_len2;
//   
//
//      return p + r * t;
//}
//
//bool Edge::IsCoplanar(const Edge& other) const
//{
//	Vector3F AB = ToVec();
//	Vector3F AC = other.A - A;
//	Vector3F AD = other.B - A;
//
//	float volume = AB.Dot(AC.Cross(AD));
//
//	return std::fabs(volume) < Math::EPSILON_FLOAT;
//}   