using UnityEngine;
using UnityEngine.UI;

public class BulletColorDisplay : MonoBehaviour
{
    private RawImage colorDisplayImage;

    void Start()
    {
        // Find the RawImage component on the same GameObject
        colorDisplayImage = GetComponent<RawImage>();

        // Set the initial color display
        SetColorDisplay(Color.red);
    }

    // Public method to update the color display
    public void SetColorDisplay(Color color)
    {
        // Set the color of the RawImage
        colorDisplayImage.color = color;
    }
}
