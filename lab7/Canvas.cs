using Godot;
using System;
using System.Collections;
using System.Collections.Generic;

public class Canvas : TextureRect
{
	public bool StateDrawCutoff { get; set; } = false;
	private bool _isInput = false;
	private bool _isShift = false;
	private Table _lines;
	private List<Tuple<Vector2Int, Vector2Int>> _linesCutted = new List<Tuple<Vector2Int, Vector2Int>>();
	private Vector2Int _previousPoint = new Vector2Int();
	private Rect _rect;

	[Export] public NodePath TablePath { get; set; }
	[Export] public NodePath RectPath { get; set; }
	[Export] public Color LinesColor { get; set; }
	[Export] public Color LinesCuttedColor { get; set; }
	[Export] public Color CutoffColor { get; set; }

	///// INITIALIZATION /////
	public override void _Ready()
	{
		_lines = GetNode<Table>(TablePath);
		_rect = GetNode<Rect>(RectPath);
		_lines.Connect("PointsChanged", this, "OnUpdate");
		_rect.Connect("Changed", this, "OnUpdate");
	}

	///// DRAW /////
	public void OnUpdate()
	{
		_linesCutted.Clear();
		Update();
	}
	public override void _Draw()
	{
		base._Draw();

		DrawRect(_rect.RectFloat, CutoffColor, false);
		if (_isInput)
		{
			Vector2 previousPoint = _previousPoint.ToVector2();
			if (StateDrawCutoff)
			{
				DrawRect(new Rect2(previousPoint, GetLocalMousePosition() - previousPoint), CutoffColor, false);
			}
			else
			{
				Vector2 mousePosition = GetLocalMousePosition();

				if (_isShift)
					mousePosition = ToStraightLine(mousePosition);

				DrawLine(previousPoint, mousePosition, LinesColor);
			}
		}

		for (int i = 0; i < _lines.Count; i++)
			DrawLine(new Vector2(_lines[i].X1, _lines[i].Y1), new Vector2(_lines[i].X2, _lines[i].Y2), LinesColor);

		foreach (Tuple<Vector2Int, Vector2Int> line in _linesCutted)
			DrawLine(line.Item1.ToVector2(), line.Item2.ToVector2(), LinesCuttedColor);
	}

	///// INPUT /////
	public override void _Input(InputEvent @event)
	{
		base._Input(@event);

		if (@event is InputEventMouseButton)
			HandleMouseButtonEvent(@event as InputEventMouseButton);
		else if (@event is InputEventMouseMotion && _isInput)
			Update();
		else if (@event is InputEventKey)
			HandleKeyButtonEvent(@event as InputEventKey);
	}
	private void HandleKeyButtonEvent(InputEventKey @event)
	{
		switch (@event.Scancode)
		{
			case (uint)KeyList.Shift:
				_isShift = @event.Pressed;
				Update();
				break;
			case (uint)KeyList.Control:
				StateDrawCutoff = @event.Pressed;
				Update();
				break;
		}
	}
	private void HandleMouseButtonEvent(InputEventMouseButton @event)
	{
		if (@event.Pressed && GetRect().HasPoint(@event.Position))
			if (@event.ButtonIndex == (int)ButtonList.Left)
				InputNode(new Vector2Int(@event.Position - RectPosition));
			else if (@event.ButtonIndex == (int)ButtonList.Right)
			{
				_isInput = false;
				Update();
			}
	}
	private void InputNode(Vector2Int position)
	{
		if (_isInput)
		{
			if (StateDrawCutoff)
			{
				Rect2Int rect = new Rect2Int(_previousPoint, position);
				rect.Abs();
				_rect.RectInt = rect;
				_rect.X2--;
				_rect.Y2--;
			}
			else
			{
				if (_isShift)
					position = ToStraightLine(position);
				_lines.AddRow(_previousPoint, position);
			}

			_isInput = false;
		}
		else
		{
			_linesCutted.Clear();
			_previousPoint = position;
			_isInput = true;
		}
	}
	private Vector2Int ToStraightLine(Vector2Int point)
	{
		if (Math.Abs(point.y - _previousPoint.y) >= Math.Abs(point.x - _previousPoint.x))
			point.x = _previousPoint.x;
		else
			point.y = _previousPoint.y;

		return point;
	}
	private Vector2 ToStraightLine(Vector2 point)
	{
		if (Math.Abs(point.y - _previousPoint.y) >= Math.Abs(point.x - _previousPoint.x))
			point.x = _previousPoint.x;
		else
			point.y = _previousPoint.y;

		return point;
	}

	///// ALGORITHM /////
	private enum LineType
	{
		Vertical,
		Horizontal,
		General
	}

	private static int BitCode(Vector2Int point, Rect2Int rect)
	{
		int result = 0;
		result |= point.x < rect.topLeft.x ? 1 : 0;
		result |= (point.x > rect.bottomRight.x ? 1 : 0) << 1;
		result |= (point.y < rect.topLeft.y ? 1 : 0) << 2;
		result |= (point.y > rect.bottomRight.y ? 1 : 0) << 3;
		return result;
	}
	private static LineType DetermineLineType(Vector2Int lineSource, Vector2Int lineDest)
	{
		LineType lineType = LineType.General;
		if (lineSource.x == lineDest.x)
			lineType = LineType.Vertical;
		else if (lineSource.y == lineDest.y)
			lineType = LineType.Horizontal;
		return lineType;
	}
	private static bool IsCodeLeft(int code) => (code & 1) != 0;
	private static bool IsCodeRight(int code) => (code & 2) != 0;
	private static bool IsCodeTop(int code) => (code & 4) != 0;
	private static bool IsCodeBottom(int code) => (code & 8) != 0;

	public void Clear()
	{
		_rect.RectInt = new Rect2Int();
		_lines.Clear();
		ClearCutted();
	}
	public void ClearCutted()
	{
		_linesCutted.Clear();
		Update();
	}

	public void CutoffCohenSutherland()
	{
		_linesCutted.Clear();
		for (int i = 0; i < _lines.Count; i++)
			CutoffCohenSutherland(new Vector2Int(_lines[i].X1, _lines[i].Y1), new Vector2Int(_lines[i].X2, _lines[i].Y2));
		Update();
	}
	private void CutoffCohenSutherland(Vector2Int pointSrc, Vector2Int pointDst)
	{
		int codeSrc = BitCode(pointSrc, _rect.RectInt);
		int codeDst = BitCode(pointDst, _rect.RectInt);

		LineType type = DetermineLineType(pointSrc, pointDst);
		float slope = (float)(pointSrc.y - pointDst.y) / (pointSrc.x - pointDst.x);

		while (codeSrc != 0 || codeDst != 0)
		{
			if (codeSrc == 0 && codeDst == 0)
			{
				_linesCutted.Add(new Tuple<Vector2Int, Vector2Int>(pointSrc, pointDst));
				return;
			}
			else if ((codeSrc & codeDst) != 0)
				return;

			int code = codeDst;
			ref Vector2Int p = ref pointDst;

			if (codeSrc != 0)
			{
				code = codeSrc;
				p = ref pointSrc;
			}

			if (IsCodeLeft(code))
			{
				p.y += (int)((_rect.X1 - p.x) * slope);
				p.x = _rect.X1;
			}
			else if (IsCodeRight(code))
			{
				p.y += (int)((_rect.X2 - p.x) * slope);
				p.x = _rect.X2;
			}
			else if (IsCodeTop(code))
			{
				p.x += (int)((_rect.Y1 - p.y) / slope);
				p.y = _rect.Y1;
			}
			else if (IsCodeBottom(code))
			{
				p.x += (int)((_rect.Y2 - p.y) / slope);
				p.y = _rect.Y2;
			}

			codeSrc = BitCode(pointSrc, _rect.RectInt);
			codeDst = BitCode(pointDst, _rect.RectInt);
		}

		_linesCutted.Add(new Tuple<Vector2Int, Vector2Int>(pointSrc, pointDst));
	}
}
