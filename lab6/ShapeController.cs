using Godot;
using System;

public class ShapeController : Button
{
	[Export] private NodePath ButtonPolylinePath { get; set; }
	[Export] private NodePath ButtonEllipsePath { get; set; }
	[Export] private NodePath TextPolylinePath { get; set; }
	[Export] private NodePath TextEllipsePath { get; set; }
	[Export] private NodePath CanvasPath { get; set; }
	private Canvas _canvas;
	private CanvasItem _textPolyline;
	private CanvasItem _textEllipse;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		_canvas = GetNode(CanvasPath) as Canvas;
		_textPolyline = GetNode(TextPolylinePath) as CanvasItem;
		_textEllipse = GetNode(TextEllipsePath) as CanvasItem;
		GetNode(ButtonPolylinePath).Connect("pressed", this, "OnPolylineButton");
		GetNode(ButtonEllipsePath).Connect("pressed", this, "OnEllipseButton");
	}

	private void OnPolylineButton()
	{
		_textEllipse.Visible = false;
		_textPolyline.Visible = true;
		_canvas.StateDrawEllipse = false;
	}

	private void OnEllipseButton()
	{
		_textEllipse.Visible = true;
		_textPolyline.Visible = false;
		_canvas.StateDrawEllipse = true;
	}
}
