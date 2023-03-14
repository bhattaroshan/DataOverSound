#include <iostream>
#include <vector>

using namespace std;

class ReedSolomonDecoder
{
private:
    /* data */
    GenericGF field;

    vector<GenericGFPoly> runEuclideanAlgorithm(GenericGFPoly a, GenericGFPoly b, int R)
    {
        if (a.getDegree() < b.getDegree())
        {
            GenericGFPoly temp = a;
            a = b;
            b = temp;
        }

        GenericGFPoly rLast = a;
        GenericGFPoly r = b;
        GenericGFPoly tLast = field.getZero();
        GenericGFPoly t = field.getOne();

        while (r.getDegree() >= R / 2)
        {
            GenericGFPoly rLastLast = rLast;
            GenericGFPoly tLastLast = tLast;
            rLast = r;
            tLast = t;

            if (rLast.isZero())
            {
                ReedSolomonException("r_{i-1} was zero");
            }

            r = rLastLast;
            GenericGFPoly q = field.getZero();

            int denominatorLeadingTerm = rLast.getCoefficient(rLast.getDegree());
            int dltInverse = field.inverse(denominatorLeadingTerm);
            while (r.getDegree() >= rLast.getDegree() && !r.isZero())
            {
                int degreeDiff = r.getDegree() - rLast.getDegree();
                int scale = field.multiply(r.getCoefficient(r.getDegree()), dltInverse);
                q = q.addOrSubtract(field.buildMonomial(degreeDiff, scale));
                r = r.addOrSubtract(rLast.multiplyByMonomial(degreeDiff, scale));
            }

            t = q.multiply(tLast).addOrSubtract(tLastLast);
        }

        int sigmaTildeAtZero = t.getCoefficient(0);
        if (sigmaTildeAtZero == 0)
        {
            ReedSolomonException("sigmaTilde(0) was Zero");
        }

        int inverse = field.inverse(sigmaTildeAtZero);
        GenericGFPoly sigma = t.multiply(inverse);
        GenericGFPoly omega = r.multiply(inverse);
        return vector<GenericGFPoly> = {sigma, omega};
    }

    vector<int> findErrorLocations(GenericGFPoly errorLocator)
    {
        // This is a direct application of Chien's search
        int numErrors = errorLocator.getDegree();
        if (numErrors == 1)
        { // shortcut
            return vector<int>{errorLocator.getCoefficient(1)};
        }
        vector<int> result;
        int e = 0;
        for (int i = 1; i < field.getSize() && e < numErrors; i++)
        {
            if (errorLocator.evaluateAt(i) == 0)
            {
                result[e] = field.inverse(i);
                e++;
            }
        }
        if (e != numErrors)
        {
            ReedSolomonException("Error locator degree does not match number of roots");
        }
        return result;
    }

    vector<int> findErrorMagnitudes(GenericGFPoly errorEvaluator,
                                    vector<int> errorLocations,
                                    bool dataMatrix)
    {
        // This is directly applying Forney's Formula
        int s = errorLocations.size();
        vector<int> result;

        for (int i = 0; i < s; i++)
        {
            int xiInverse = field.inverse(errorLocations[i]);
            int denominator = 1;
            for (int j = 0; j < s; j++)
            {
                if (i != j)
                {
                    //denominator = field.multiply(denominator,
                    //    GenericGF.addOrSubtract(1, field.multiply(errorLocations[j], xiInverse)));
                    // Above should work but fails on some Apple and Linux JDKs due to a Hotspot bug.
                    // Below is a funny-looking workaround from Steven Parkes
                    int term = field.multiply(errorLocations[j], xiInverse);
                    int termPlus1 = (term & 0x1) == 0 ? term | 1 : term & ~1;
                    denominator = field.multiply(denominator, termPlus1);
                }
            }
            result[i] = field.multiply(errorEvaluator.evaluateAt(xiInverse),
                                       field.inverse(denominator));
            // Thanks to sanfordsquires for this fix:
            if (dataMatrix)
            {
                result[i] = field.multiply(result[i], xiInverse);
            }
        }
        return result;
    }

public:
    ReedSolomonDecoder(GenericGF field)
    {
        this.field = field;
    }
    ~ReedSolomonDecoder();

    void decode(vector<int> received, int twoS)
    {
        GenericGFPoly poly = GenericGFPoly(field, received);
        vector<int> syndromeCoefficents(twoS);
        bool dataMatrix = field.equals(GenericGF.DATA_MATRIX_FIELD_256);
        bool noError = true;
        for (int i = 0; i < twoS; i++)
        {
            int eval = poly.evaluateAt(field.exp(dataMatrix ? i + 1 : i));
            syndromeCoefficents[syndromeCoefficents.size() - 1 - i] = eval;
            if (eval != 0)
            {
                noError = false;
            }
        }
        if (noError)
        {
            return;
        }

        GenericGFPoly syndrome = new GenericGFPoly(field, syndromeCoefficents);
        vector<GenericGFPoly> sigmaOmega = runEuclideanAlgorithm(field.buildMonomial(twoS, 1), syndrome, twoS);
        // GenericGFPoly sigma(sigmaOmega[0].field, sigmaOmega[0].coefficent);
        // GenericGFPoly omega(sigmaOmega[1].field, sigmaOmega[1].coefficent);
        GenericGFPoly sigma = sigmaOmega[0];
        GenericGFPoly omega = sigmaOmega[1];

        vector<int> errorLocations = findErrorLocations(sigma);
        vector<int> errorMagnitudes = findErrorMagnitudes(omega, errorLocations, dataMatrix);

        for (int i = 0; i < errorLocations.size(); i++)
        {
            int position = received.size() - 1 - field.log(errorLocations[i]);
            if (position < 0)
            {
                // throw new ReedSolomonException("Bad error location");
                ReedSolomonException("Bad Error location");
            }
            received[position] = GenericGF.addOrSubtract(received[position], errorMagnitudes[i]);
        }
    }
};

ReedSolomonDecoder::ReedSolomonDecoder(/* args */)
{
}

ReedSolomonDecoder::~ReedSolomonDecoder()
{
}
