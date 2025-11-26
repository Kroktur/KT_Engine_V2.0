//#include <map>
//
//#include "Delaunay.h"
//Triangle PointToTriangle(const Point3F& A, const Point3F& B, const Point3F& C)
//{
//	Vertex Av, Bv, Cv;
//	Av.position = A;
//	Bv.position = B;
//	Cv.position = C;
//	return Triangle{ Av, Bv, Cv };
//}
//std::vector<Triangle> GenerateTriangles(const std::vector<Edge>& edges, const Point3F& p)
//{
//	std::vector<Triangle> result;
//	for (auto& e : edges)
//	{
//		result.push_back(PointToTriangle(e.A, e.B, p));
//	}
//	return result;
//}
//
//
//bool SharedTriangles(const Triangle& t, std::vector<Triangle> vec)
//{
//	for (auto& triangle : vec)
//	{
//		if (t.HasSamePoint(triangle))
//			return true;
//	}
//	return false;
//}
//
//std::vector<Edge> ExtractEdge(const std::vector<Triangle>& triangle)
//{
//	std::vector<Edge> edges;
//	for (auto& t : triangle)
//	{
//		auto tedge = t.GetEdges();
//		edges.insert(edges.end(), tedge.begin(), tedge.end());
//	}
//
//	std::vector<Edge> result;
//	for (auto& e : edges)
//	{
//		int count = 0;
//		for (auto& e2 : edges)
//		{
//			if (e == e2)
//				count++;
//		}
//		if (count == 1)
//			result.push_back(e);
//	}
//	return result;
//}
//
//std::vector<Edge> ExtractAllEdge(const std::vector<Triangle>& triangles)
//{
//	std::vector<Edge> edges;
//
//	auto addUnique = [&](const Edge& e)
//		{
//			for (auto& existing : edges)
//			{
//				if (existing == e)
//					return;
//			}
//			edges.push_back(e);
//		};
//
//	for (const auto& t : triangles)
//	{
//		addUnique({ t.points[0].position, t.points[1].position });
//		addUnique({ t.points[1].position, t.points[2].position });
//		addUnique({ t.points[2].position, t.points[0].position });
//	}
//
//	return edges;
//}
//
//std::vector<Point3F> ExtractAllPoints(const std::vector<Triangle>& triangles)
//{
//	std::vector<Point3F> points;
//
//	auto addUnique = [&](const Point3F& e)
//		{
//			for (const auto& existing : points)
//			{
//				if (existing == e)
//					return;
//			}
//			points.push_back(e);
//		};
//
//	for (const auto& t : triangles)
//	{
//		addUnique(t.points[0].position);
//		addUnique(t.points[1].position);
//		addUnique(t.points[2].position);
//	}
//
//	return points;
//}
//
//bool SharedEdges(const Triangle& t, const Edge& edge)
//{
//	auto edges = t.GetEdges();
//	if (edge == edges[0] || edge == edges[1] || edge == edges[2])
//		return  true;
//	return false;
//}
//
//Delaunay::DelaunayInfo Delaunay::Triangulate(const std::vector<Point3F>& points)
//{
//	auto delunayInfo = TriangulateFirstStep(points);
//	EraseSuperTriangleEdges(delunayInfo);
//	return delunayInfo;
//}
//
//Delaunay::DelaunayInfo Delaunay::ConstraintTriangulate(const std::vector<Point3F>& points, std::vector<Edge> border)
//{
//auto delunayInfo = TriangulateFirstStep(points);
//Constraint(delunayInfo,border);
//EraseSuperTriangleEdges(delunayInfo);
//return delunayInfo;
//}
//
//
//void Delaunay::EraseInternalTriangle(DelaunayInfo& triangulation, std::vector<Edge> border)
//{
//
//	std::map<size_t, bool> isErased;
//	for (int i = 0; i < triangulation.triangle.size(); ++i)
//	{
//		if (isIn(triangulation.triangle[i].GetCenter(), border))
//			isErased[i] = true;
//	}
//	std::vector<Triangle> result;
//	for (int i = 0; i < triangulation.triangle.size(); ++i)
//	{
//		if (!isErased[i])
//			result.push_back(triangulation.triangle[i]);
//	}
//	triangulation.triangle = std::move(result);
//}
//
//Delaunay::DelaunayInfo Delaunay::TriangulateFirstStep(const std::vector<Point3F>& points)
//{
//	auto superTriangle = GetSuperTriangle(points);
//	std::vector<Triangle> ActualTriangle;
//	for (auto& p : points)
//	{
//		std::vector<Triangle> badTriangles;
//		for (auto& t : ActualTriangle)
//			if (t.IsInCircumcenter(p))
//				badTriangles.push_back(t);
//		std::vector<Triangle> tmp;
//		for (auto& t : ActualTriangle)
//		{
//			if (SharedTriangles(t, badTriangles))
//				continue;
//			tmp.push_back(t);
//		}
//		ActualTriangle = std::move(tmp);
//		if (ActualTriangle.empty())
//		{
//			auto triangles = GenerateTriangles(superTriangle.GetEdges(), p);
//			ActualTriangle.insert(ActualTriangle.end(), triangles.begin(), triangles.end());
//		}
//
//		auto triangles = GenerateTriangles(ExtractEdge(badTriangles), p);
//		ActualTriangle.insert(ActualTriangle.end(), triangles.begin(), triangles.end());
//	}
//	return Delaunay::DelaunayInfo(superTriangle, ActualTriangle);
//}
//
//void Delaunay::Constraint(DelaunayInfo& triangulation, std::vector<Edge> border)
//{
//	// constraint here
//	auto& ActualTriangle = triangulation.triangle;
//	auto edges = ExtractAllEdge(ActualTriangle);
//
//	for (auto& b : border)
//	{
//		std::vector<Edge> badEdges;
//		std::vector<Triangle> badTriangle;
//		std::vector<Point3F> IntersectPoints;
//		IntersectPoints.push_back(b.A);
//		IntersectPoints.push_back(b.B);
//
//		for (auto& edge : edges)
//		{
//			if (b.IsIntersect(edge) && b != edge)
//			{
//				if (b.A == edge.A || b.A == edge.B || b.B == edge.A || b.B == edge.B)
//					continue;
//				IntersectPoints.push_back(b.InterSect(edge));
//				badEdges.push_back(edge);
//			}
//		}
//		if (badEdges.empty())
//			continue;
//
//		std::map<size_t, bool> badTriangleIndex;
//		for (auto& be : badEdges)
//		{
//			for (int i = 0; i < ActualTriangle.size(); ++i)
//			{
//				if (SharedEdges(ActualTriangle[i], be))
//					badTriangleIndex[i] = true;
//			}
//		}
//		for (int i = 0; i < ActualTriangle.size(); ++i)
//		{
//			if (badTriangleIndex.contains(i))
//				badTriangle.push_back(ActualTriangle[i]);
//		}
//		// extraires les points
//		auto points = ExtractAllPoints(badTriangle);
//		// enlever les mauvais triangle
//		std::vector<Triangle> tmp;
//		for (int i = 0; i < ActualTriangle.size(); ++i)
//		{
//			if (!badTriangleIndex.contains(i))
//				tmp.push_back(ActualTriangle[i]);
//		}
//		ActualTriangle = std::move(tmp);
//		// trier gauche droite
//		std::vector<Point3F> leftPoints;
//		std::vector<Point3F> rightPoints;
//		leftPoints = IntersectPoints;
//		rightPoints = IntersectPoints;
//		Vector3F dir = b.B - b.A;
//
//		for (auto& p : points)
//		{
//			if (p == b.A || p == b.B) continue;
//			Vector3F rel = p - b.A;
//			Vector3F cross = dir.Cross(rel);
//			if (cross.y < 0)
//				leftPoints.push_back(p);
//			else
//				rightPoints.push_back(p);
//		}
//		// trianguler 
//		if (leftPoints.size() >= 3)
//		{
//			auto p = Triangulate(leftPoints);
//			ActualTriangle.insert(ActualTriangle.end(), p.triangle.begin(), p.triangle.end());
//		}
//		if (rightPoints.size() >= 3)
//		{
//			auto pointxs = Triangulate(rightPoints).triangle;
//			int toto = 4;
//			ActualTriangle.insert(ActualTriangle.end(), pointxs.begin(), pointxs.end());
//		}
//
//	}
//}
//
//void Delaunay::EraseSuperTriangleEdges(DelaunayInfo& triangulation)
//{
//	auto& ActualTriangle = triangulation.triangle;
//	auto superTriangle = triangulation.superTriangle;
//	std::vector<Triangle> tmp;
//	for (auto& t : ActualTriangle)
//	{
//		if (t.HasPoint(superTriangle.points[0].position) || t.HasPoint(superTriangle.points[1].position) || t.HasPoint(superTriangle.points[2].position))
//			continue;
//		tmp.push_back(t);
//	}
//	ActualTriangle = std::move(tmp);
//}
//
//Triangle Delaunay::GetSuperTriangle(const std::vector<Point3F>& points)
//{
//	AABB3DF box(points[0], points[0]);
//	for (auto& p : points)
//		box.Expand(p);
//	box.Amin -= Vector3F{ 10,0,10 };
//	box.Amax += Vector3F{ 10,0,10 };
//
//	Vector3F A = Vector3F{ box.Amin.x - box.HalfSize().x,box.Amin.y,box.Amin.z };
//	Vector3F B = Vector3F{ box.Amax.x + box.HalfSize().x,box.Amin.y,box.Amin.z };
//
//
//	Vector3F Aprime = Vector3F{ box.Amin.x,box.Amin.y,box.Amax.z };
//	Vector3F lhsDir = Vector3F(A, Aprime).normalize();
//
//	float Cx = box.Center().x;
//	float Cy = box.Amin.y;
//	float Cz = A.z + lhsDir.z * ((Cx - A.x) / lhsDir.x);
//	Vertex Av, Bv, Cv;
//	Av.position = A;
//	Bv.position = B;
//	Cv.position = Vector3F(Cx, Cy, Cz);
//	return Triangle(Av, Bv, Cv);
//}
//
//bool Delaunay::isIn(const Point3F& P, std::vector<Edge> border)
//{
//	float z = P.z;
//	float x = P.x;
//	float y = P.y;
//	int count = 0;
//	for (auto& b : border)
//	{
//		if (b.ToVec().normalize() == Vector3F::Dir<Dir::RIGHT>() || b.ToVec().normalize() == Vector3F::Dir<Dir::LEFT>())
//			continue;
//		float maxx = std::max(b.A.x, b.B.x);
//		float minz = std::min(b.A.z, b.B.z);
//		float maxz = std::max(b.A.z, b.B.z);
//		float miny = std::min(b.A.y, b.B.y);
//		float maxy = std::max(b.A.y, b.B.y);
//		if (maxx < x)
//			continue;
//		if (z < minz || z > maxz)
//			continue;
//		if (y < miny || y > maxy)
//			continue;
//		++count;
//	}
//	return (count % 2 == 0) ? true : false;
//}
//
//
