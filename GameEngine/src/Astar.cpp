//#include "Astar.h"
//
//struct Astar::Track Astar::GenerateTrack(const Point3F& start, const Point3F& end, const FinalRegion& region)
//{
//	// start width find the first and last triangle
//	Astar::Track result;
//	result.start = start;
//	result.end = end;
//	std::optional<Triangle> startT;
//	std::optional < Triangle> endT;
//	for (auto& triangle : region.triangles)
//	{
//		if (triangle.testPointTriangle(start))
//		{
//			startT = triangle;
//		}
//		if (triangle.testPointTriangle(end))
//			endT = triangle;
//	}
//	if (!startT.has_value() || !endT.has_value())
//		throw std::runtime_error("track impossible");
//	// calculate f and g for the rest of the node
//	const float  g = (end - start).Length();
//	const float f = g * 2.0f;
//
//	// 2 list 
//	AstarList<Triangle> open;
//	AstarList<Triangle> closed;
//
//	open.Add(&startT.value(), f, nullptr);
//
//	while (!open.Empty())
//	{
//		// pop open into closed to never visit again
//		auto* nodetest = open.Front();
//		AstarList<Triangle>::Node node = AstarList<Triangle>::Node(nodetest->value, nodetest->f, nodetest->parent);
//		open.PopFront();
//		closed.Add(node.value, node.f, node.parent);
//		if (*node.value == endT)
//		{
//			// find from last and revert
//			auto& vec = result.triangles;
//			auto* current = &node;
//			while (current != nullptr)
//			{
//				vec.push_back(*current->value);
//				if (!current->parent)
//					current = nullptr;
//				else
//				{
//					current = closed.Get(current->parent);
//				}
//			}
//
//			std::reverse(vec.begin(), vec.end());
//			return result;
//		}
//		auto& triangle = node.value;
//		for (auto* neighbor : triangle->neighbors)
//		{
//			if (closed.contain(neighbor))
//				continue;
//			if (open.contain(neighbor))
//				continue;
//			float h = (end - neighbor->GetCenter()).Length() ;
//			// add Node
//			open.Add(neighbor, h + g, node.value);
//		}
//
//	}
//	return result;
//}
//
//Astar::FinalTrack Astar::TrackThroughCenter(const Track& track)
//{
//	Astar::FinalTrack ftrack;
//	ftrack.start = track.start;
//	ftrack.end = track.end;
//	ftrack.points.push_back(track.start);
//	for (auto& triangle : track.triangles)
//	{
//		ftrack.points.push_back(triangle.GetCenter());
//	}
//	ftrack.points.push_back(track.end);
//
//	if (ftrack.points.front() == ftrack.points[1])
//		ftrack.points.erase(ftrack.points.begin());
//
//	if (ftrack.points.back() == ftrack.points[ftrack.points.size() - 2])
//		ftrack.points.erase(ftrack.points.end() - 1);
//	return ftrack;
//}
//
//static Edge ExtractCommonEdge(const Triangle& lhs, const Triangle& rhs)
//{
//	auto lhsEdge = lhs.GetEdges();
//	auto rhsEdge = rhs.GetEdges();
//
//	for (auto& e : lhsEdge)
//	{
//		for (auto& e2 : rhsEdge)
//		{
//			if (e == e2)
//				return e;
//		}
//	}
//	throw std::out_of_range("triangle dont touch");
//}
//
//Astar::FinalTrack Astar::FunelTrack(const Track& track)
//{
//	Astar::FinalTrack ftrack;
//	ftrack.start = track.start;
//	ftrack.end = track.end;
//
//	ftrack.points.push_back(track.start);
//
//	if (track.triangles.size() >= 2)
//	{
//
//		for (int i = 0; i < track.triangles.size() - 1; ++i)
//		{
//			auto e = ExtractCommonEdge(track.triangles[i], track.triangles[i + 1]);
//			Point3F center = e.A + (e.ToVec() / 2);
//			ftrack.points.push_back(center);
//		}
//	}
//	ftrack.points.push_back(track.end);
//
//
//	if (ftrack.points.front() == ftrack.points[1])
//		ftrack.points.erase(ftrack.points.begin());
//
//	if (ftrack.points.back() == ftrack.points[ftrack.points.size() - 2])
//		ftrack.points.erase(ftrack.points.end() - 1);
//	return ftrack;
//}
//
//Astar::FinalTrack Astar::BezierTrack(const Track& track, size_t PointCount)
//{
//	if (PointCount < 1)
//		throw std::out_of_range(" at least 1 point");
//	auto ftrack = FunelTrack(track);
//	if (ftrack.points.size() < 3)
//		return ftrack;
//	std::vector<Point3F> pts;
//	pts.push_back(ftrack.points.front());
//	for (int i = 1; i < ftrack.points.size() - 2; ++i)
//	{
//		pts.push_back(ftrack.points[i]);
//		Vector3F center = ftrack.points[i] + ((ftrack.points[i + 1] - ftrack.points[i]) / 2.0f);
//		pts.push_back(center);
//	}
//	pts.push_back(ftrack.points[ftrack.points.size() - 2]);
//	pts.push_back(ftrack.points.back());
//
//	std::vector<Point3F> fpts;
//	fpts.push_back(ftrack.start);
//	for (int i = 0; i + 2 < static_cast<int>(pts.size()); i += 2)
//	{
//		auto& start = pts[i];
//		auto& control = pts[i + 1];
//		auto& end = pts[i + 2];
//
//		// add 1 and start at 1
//		float step = 1.0f / (static_cast<float>(PointCount));
//		for (int j = 1; j <= static_cast<int>(PointCount); ++j)
//		{
//			float t = j * step;
//
//			float L1x = Math::Lerp(start.x, control.x, t);
//			float L1y = Math::Lerp(start.y, control.y, t);
//			float L1z = Math::Lerp(start.z, control.z, t);
//
//
//			float L2x = Math::Lerp(control.x, end.x, t);
//			float L2y = Math::Lerp(control.y, end.y, t);
//			float L2z = Math::Lerp(control.z, end.z, t);
//
//			float x = Math::Lerp(L1x, L2x, t);
//			float y = Math::Lerp(L1y, L2y, t);
//			float z = Math::Lerp(L1z, L2z, t);
//
//			fpts.push_back(Point3F(x, y, z));
//		}
//
//
//	}
//	ftrack.points = std::move(fpts);
//	return ftrack;
//}
//
//
//
