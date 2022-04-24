using Godot;
using System;

public class ColorPicker : ColorRect
{
	Canvas _canvas;
	[Export] private NodePath ButtonBlack { get; set; }
	[Export] private NodePath ButtonRed { get; set; }
	[Export] private NodePath ButtonGreen { get; set; }
	[Export] private NodePath ButtonBlue { get; set; }
	[Export] private NodePath CanvasPath { get; set; }

	public override void _Ready()
	{
		_canvas = GetNode(CanvasPath) as Canvas;
		GetNode(ButtonBlack).Connect("pressed", this, "SetColorBlack");
		GetNode(ButtonRed).Connect("pressed", this, "SetColorRed");
		GetNode(ButtonGreen).Connect("pressed", this, "SetColorGreen");
		GetNode(ButtonBlue).Connect("pressed", this, "SetColorBlue");
	}
	private void SetColorBlack() => NewColor(0, 0, 0);
	private void SetColorRed() => NewColor(1, 0, 0);
	private void SetColorGreen() => NewColor(0, 1, 0);
	private void SetColorBlue() => NewColor(0, 0, 1);
	private void NewColor(float r, float g, float b)
	{
		Color = new Color(r, g, b);
		_canvas.ActiveColor = Color;
	}
}
