using Godot;
using System.Collections.Generic;

public class Table : VBoxContainer
{
	[Signal] delegate void PointsChanged();
	List<Row> _rows = new List<Row>();
	private PackedScene _rowPacked = GD.Load<PackedScene>("res://Row.tscn");

	public Row this[int i]
	{
		get => _rows[i];
		set => _rows[i] = value;
	}

	public int Count
	{
		get => _rows.Count;
	}

	public override void _Ready()
	{

	}

	public void AddRow() => AddRow(new Vector2Int(0, 0), new Vector2Int(0, 0));
	public void AddRow(Vector2Int p1, Vector2Int p2)
	{
		Row row = _rowPacked.Instance<Row>();
		row._Ready();
		row.Index = _rows.Count;
		row.X1 = p1.x;
		row.Y1 = p1.y;
		row.X2 = p2.x;
		row.Y2 = p2.y;
		row.Name = "Row" + _rows.Count;
		AddChild(row);

		_rows.Add(row);

		row.Connect("Changed", this, "UpdatePoint");
		row.Connect("Destroy", this, "RemovePoint");
	}

	public void UpdatePoint()
		=> EmitSignal("PointsChanged");

	public void RemovePoint(int index)
	{
		_rows[index].QueueFree();
		_rows.RemoveAt(index);
		for (int i = index; i < _rows.Count; i++)
		{
			_rows[i].Index--;
			_rows[i].Name = "Row" + _rows[i].Index;
		}
		EmitSignal("PointsChanged");
	}
	public void Clear()
	{
		for (int i = 0; i < Count; i++)
			_rows[i].QueueFree();

		_rows.Clear();
	}
}
